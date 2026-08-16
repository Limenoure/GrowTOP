#include <BaseApp.hpp> //precomp

#include <fstream>

#include <Network/WebServer.hpp>
#include <ProtonSDK/TextScanner.h>

WebServer g_webServer;
WebServer* GetWebServer() { return &g_webServer; }

WebServer::~WebServer()
{
	if (m_pSslServer)
	{
		m_pSslServer->stop();
		delete m_pSslServer;
		m_pSslServer = nullptr;
	}

	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

bool WebServer::Init(const char* address, const uint16_t& port, const eWebServerType& type)
{
	switch (type)
	{
	    case eWebServerType::TYPE_SSL:
		{
			if (m_pSslServer)
			{
				return false;
			}

			m_pSslServer = new httplib::SSLServer(std::string("web/server.key").c_str(), std::string("web/key.pem").c_str());
			if (!m_pSslServer->bind_to_port(address, port))
			{
				delete m_pSslServer; // Ensure cleanup on failure
				m_pSslServer = nullptr;
				return false;
			}

			m_pSslServer->set_post_routing_handler([](const httplib::Request& req, httplib::Response& res)
				{
					res.set_header("Access-Control-Allow-Origin", "*");
					res.set_header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
				});

			m_thread = std::thread(&WebServer::Serve, this);
			return true;
		} break;
	}

	return false;
}

void WebServer::Serve()
{
	if (!m_pSslServer)
	{
		return;
	}

	m_pSslServer->Post("/growtopia/server_data.php", [&](const httplib::Request& req, httplib::Response& res)
		{
			if (req.params.empty() || req.get_header_value("User-Agent").find("UbiServices_SDK") == std::string::npos)
			{
				res.status = 403;
				return;
			}

			std::string data;
			data.append("server|" + GetBaseApp()->GetConfig().address + "\n");
			data.append("loginurl|login.yoruakio.tech");
			data.append("port|" + std::to_string(GetBaseApp()->GetConfig().port) + "\n");
			data.append("type|1\n");
			data.append("type2|0\n");
			if (!GetBaseApp()->IsRunning())
			{
				data.append("maint|Server is under maintenance. We will be back online shortly. Thank you for your patience!\n");
			}

			data.append("meta|vytenis kalaciovas & Jolita Kalacioveine & Austeja Kalacioveine\n");
			data.append("RTENDMARKERBS1001\n\n");
			res.set_content(data, "text/html");
			return;
		});

	m_pSslServer->Post("/player/login/dashboard", [&](const httplib::Request& req, httplib::Response& res)
		{
			auto load_file_content = [&](const std::string& path) -> std::string
				{
					std::ifstream file(path);
					if (file.is_open()) 
					{
						std::stringstream buffer;
						buffer << file.rdbuf();
						return buffer.str();
					}

					return "";
				};

			std::string content = load_file_content("public/html/dashboard.html");
			res.set_content(content, "text/html");
		});

	m_pSslServer->Post("/player/growid/login/validate", [&](const httplib::Request& req, httplib::Response& res)
		{
			std::string loginToken = req.get_param_value("_token");
			std::string growID = req.get_param_value("growId");
			std::string password = req.get_param_value("password");

			std::string token = base64_encode("_token=" + loginToken + "&growId=" + growID + "&password=" + password);

			if (GetOnlineManager()->GetLogonsThisSecond() >= GetBaseApp()->GetConfig().maxConnectionsPerSec)
			{ 
				//too much logons per second
				res.set_content("{\"status\":\"failed\",\"message\":\"Oops, too many people logging at once.\",\"token\"\"" + token + "\",\"url\":"",\"accountType\":\"growtopia\"}", "application/json");
				return;
			}

			res.set_content("{\"status\":\"success\",\"message\":\"Account Validated.\",\"token\":\"" + token + "\",\"url\":\"\",\"accountType\":\"growtopia\"}", "application/json");
		});

	m_pSslServer->Post("/player/validate/close", [&](const httplib::Request& req, httplib::Response& res)
		{
			res.set_content("<script>window.close();</script>", "text/html");
		});

	m_pSslServer->listen_after_bind();
}