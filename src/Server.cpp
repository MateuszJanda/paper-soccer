#include "Server.hpp"
#include <iostream>
#include <memory>
#include <functional>

namespace PaperSoccer {

Server::Server(boost::asio::io_context &ioContext, const boost::asio::ip::tcp::endpoint& endpoint)
    : m_ioContext{ioContext},
      m_acceptor{ioContext, endpoint},
      m_desc{ioContext, 0},
      m_ncurses{},
      m_board{8, 10},
      m_view{m_board, m_ncurses}
{
//    std::cout << "tutaj " << "\n";
    accept();
}

void Server::accept()
{
    m_acceptor.async_accept(
        [this](boost::system::error_code errorCode, boost::asio::ip::tcp::socket socket)
        {
            if (not errorCode) {
//                std::cout << "accept" << "\n";
                m_game = std::make_shared<Game>(m_board, m_ncurses, m_view, std::move(socket));
                m_game->readMsg();
                setupInputLoop();
            }

        });
}

void Server::inputLoop(boost::system::error_code errorCode)
{
    using namespace std::placeholders;

    if (not errorCode) {
        m_game->on_input();
        m_desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read, std::bind(&Server::inputLoop, this, _1));
    } else {
//        std::cout << "error" << "\n";
    }
}

void Server::setupInputLoop()
{
    inputLoop(boost::system::error_code{});
}

}
