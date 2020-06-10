#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Network.hpp"
#include <boost/asio.hpp>

namespace PaperSoccer {

class Client : public Network {
public:
    Client(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::resolver::results_type& endpoints);

    void run() override;
    void connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);

private:
    const boost::asio::ip::tcp::resolver::results_type& m_endpoints;
};

} // namespace PaperSoccer

#endif // CLIENT_HPP
