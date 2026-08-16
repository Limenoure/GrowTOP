#include <BaseApp.hpp> //precomp

#include <Network/NetClient.hpp>

io_context g_context2;
NetClient g_netClient(g_context2, "0.0.0.0", "16999");
NetClient* GetNetClient() { return &g_netClient; }


NetClient::NetClient(io_context& context, const std::string& host, const std::string& port) : m_context(context), m_resolver(context), m_socket(context)
{
    ip::tcp::resolver::results_type endpoints = m_resolver.resolve(host, port);
    boost::asio::async_connect(m_socket, endpoints, [this](const boost::system::error_code& error, const boost::asio::ip::tcp::endpoint& /*endpoint*/)
    {
        HandleConnect(error);
    });
}

NetClient::~NetClient()
{
    m_socket.close();
}

void NetClient::Connect()
{
    m_context.run();
}

void NetClient::SendAuthKey(const std::string& authKey)
{
    boost::asio::async_write(m_socket, boost::asio::buffer(authKey), [this](const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        HandleWrite(error, bytes_transferred);
    });
}

void NetClient::HandleConnect(const boost::system::error_code& error)
{
    if (!error)
    {
        printf("connected to master server.\n");
    }
    else
    {
        printf("failed to connect to master server\nError: %s.\n", error.message().c_str());
    }
}

void NetClient::HandleWrite(const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
{
    if (!error)
    {
        boost::asio::async_read(m_socket, boost::asio::buffer(m_buffer), [this](const boost::system::error_code& error, std::size_t bytes_transferred)
        {
            HandleRead(error, bytes_transferred);
        });
    }
    else
    {
        printf("failed to send auth request.\nError: %s.\n", error.message().c_str());
    }
}

void NetClient::HandleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error)
    {
        std::string response(m_buffer.data(), bytes_transferred);
        printf("%s\n", response.c_str());
    }
    else
    {
        printf("failed to read response.\nError: %s.\n", error.message().c_str());
    }
}