#include <BaseApp.hpp> //precomp

#include "NetServer.hpp"

io_context g_context;
NetServer g_netServer(g_context, 16999);
NetServer* GetNetServer() { return &g_netServer; }

NetServer::NetServer(io_context& ctx, const uint16_t& port)
    : m_context(ctx), m_acceptor(ctx, ip::tcp::endpoint(ip::tcp::v4(), port))
{
}

NetServer::~NetServer()
{
    for (Socket& s : m_sockets)
    {
        if (s.pSocket && s.pSocket->is_open())
        {
            s.pSocket->close();
        }
    }
    m_sockets.clear();
}

void NetServer::Listen()
{
    Accept();
}

void NetServer::Accept()
{
    auto socket = std::make_shared<ip::tcp::socket>(m_context);
    m_acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& error)
        {
            if (error)
            {
                LogError("Failed to accept connection (%s)", error.message().c_str());
                return;
            }

            Socket s;
            s.bAuthenticated = false;
            s.pSocket = socket;

            boost::asio::async_read(*socket, boost::asio::buffer(s.buffer), [this, s](const boost::system::error_code& error, std::size_t length) mutable
                {
                    if (error)
                    {
                        LogError("Failed to read from socket (%s)", error.message().c_str());
                        s.pSocket->close();
                        return;
                    }

                    std::string authKey(s.buffer.data(), length);
                    if (s.ValidateAuthKey(authKey))
                    {
                        s.bAuthenticated = true;
                        m_sockets.push_back(s);
                        LogMsg("Client authenticated successfully.");
                    }
                    else
                    {
                        boost::asio::async_write(*s.pSocket, boost::asio::buffer("Failed to authenticate."), [this, s](const boost::system::error_code& write_error, std::size_t /*bytes_transferred*/) mutable
                            {
                                if (write_error)
                                {
                                    LogError("Failed to send auth failure message (%s)", write_error.message().c_str());
                                }

                                s.pSocket->close();
                            });
                    }

                    Listen();
                });
        });
}