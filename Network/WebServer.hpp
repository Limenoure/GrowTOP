#ifndef NETWORK__WEBSERVER__HPP
#define NETWORK__WEBSERVER__HPP

#include <string>
#include <thread>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <cpp-httplib/httplib.h>

enum class eWebServerType : uint8_t
{
	TYPE_HTTP,
	TYPE_SSL
};

class WebServer 
{
public:
	WebServer() 
	{
		m_pServer = nullptr;
		m_pSslServer = nullptr;
	}
	~WebServer();

	eWebServerType GetType() const { return m_type; }
	void SetType(const eWebServerType& type) { m_type = type; }

	bool Init(const char* address, const uint16_t& port, const eWebServerType& type);
	void Serve();

private:
	httplib::Server* m_pServer = nullptr;
	httplib::SSLServer* m_pSslServer = nullptr;

	eWebServerType m_type;
	std::thread m_thread;

};

WebServer* GetWebServer();

#endif // NETWORK__WEBSERVER__HPP