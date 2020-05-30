#include "Server.hpp"
#include <iostream>
#include <memory>

namespace PaperSoccer {

Server::Server(boost::asio::io_context &ioContext, const boost::asio::ip::tcp::endpoint& endpoint)
    : m_ioContext{ioContext},
      m_acceptor{ioContext, endpoint},
      m_ncurses{},
      m_board{8, 10},
      m_view{m_board, m_ncurses}
{
    m_game = std::make_shared<Game>(m_board, m_ncurses, m_view);
    setInputLoop();
//    accept();
}

void Server::accept()
{
    m_acceptor.async_accept(
        [this](boost::system::error_code errorCode, boost::asio::ip::tcp::socket socket)
        {
            if (not errorCode) {
                std::cout << "accept" << "\n";
//                m_game = std::make_shared<Game>(m_board, m_ncurses, m_view);
//                setInputLoop();
            }

        });
}

void Server::onInput()
{
    m_game->on_input();
}


void Server::setInputLoop()
{
    std::function<void(boost::system::error_code)> inputLoop;

    boost::asio::posix::stream_descriptor desc(m_ioContext, 0);
    inputLoop = [&, this](boost::system::error_code errorCode) {
        if (not errorCode) {
            onInput();
            desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read, inputLoop);
        } else {
            std::cout << "error" << "\n";
        }
    };

//    std::cout << "input loop" << "\n";
    inputLoop(boost::system::error_code{});
}

}
