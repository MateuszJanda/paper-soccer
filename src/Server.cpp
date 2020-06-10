#include "Server.hpp"
#include <memory>

namespace PaperSoccer {

Server::Server(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint)
    : Network{ioContext}
    , m_acceptor{ioContext, endpoint}
{
}

void Server::run()
{
    accept();
}

void Server::accept()
{
    m_acceptor.async_accept(m_socket,
        [this](boost::system::error_code errorCode) {
            if (not errorCode) {
                setupHandlers();
            }
        });
}

} // namespace PaperSoccer
