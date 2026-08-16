#ifndef NETWORK__NETSERVER__HPP
#define NETWORK__NETSERVER__HPP
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <boost/asio.hpp>

using namespace boost::asio;

struct Socket
{
    bool bAuthenticated = false;
    std::shared_ptr<ip::tcp::socket> pSocket;
    std::array<char, 256> buffer; // Buffer to read the auth key into

    bool ValidateAuthKey(const std::string& authKey)
    {
        return authKey == "authKey_NetworkServer1";
    }
};

class NetServer
{
public:
    NetServer(io_context& ctx, const uint16_t& port);
    ~NetServer();

    io_context& GetContext() { return m_context; }

    void Listen();

private:
    void Accept();

    io_context& m_context;
    ip::tcp::acceptor m_acceptor;
    std::vector<Socket> m_sockets;
};

NetServer* GetNetServer();

#endif // NETWORK__NETSERVER__HPP