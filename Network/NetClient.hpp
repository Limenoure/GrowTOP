#ifndef NETWORK__NETCLIENT__HPP
#define NETWORK__NETCLIENT__HPP
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;

class NetClient
{
public:
    NetClient(boost::asio::io_context& context, const std::string& host, const std::string& port);
    ~NetClient();

    void Connect();
    void SendAuthKey(const std::string& authKey);

private:
    void HandleConnect(const boost::system::error_code& error);
    void HandleWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
    void HandleRead(const boost::system::error_code& error, std::size_t bytes_transferred);

    io_context& m_context;
    ip::tcp::resolver m_resolver;
    ip::tcp::socket m_socket;
    std::array<char, 256> m_buffer;
};

NetClient* GetNetClient();

#endif // NETWORK__NETCLIENT__HPP