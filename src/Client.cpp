#include "Client.hpp"

namespace PaperSoccer {


Client::Client(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::resolver::results_type& endpoints)
    : m_ioContext{ioContext},
      m_socket{ioContext},
      m_desc{ioContext, 0},
      m_ncurses{},
      m_board{8, 10},
      m_view{m_board, m_ncurses}
{
    connect(endpoints);
}

void Client::connect(const boost::asio::ip::tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, endpoints,
        [this](boost::system::error_code errorCode, boost::asio::ip::tcp::endpoint)
        {
            if (not errorCode)
            {
                m_game = std::make_shared<Game>(m_board, m_ncurses, m_view);
                setupInputLoop();

                m_game->readInitMsg();
            }
        });
}

void Client::inputLoop(boost::system::error_code errorCode)
{
    using namespace std::placeholders;

    if (not errorCode) {
        m_game->on_input();
        m_desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read, std::bind(&Client::inputLoop, this, _1));
    }
}


void Client::setupInputLoop()
{
    inputLoop(boost::system::error_code{});
}


}
