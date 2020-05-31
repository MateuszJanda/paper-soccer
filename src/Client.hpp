#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
#include "NCurses.hpp"
#include "View.hpp"
#include "Board.hpp"
#include "Game.hpp"

namespace PaperSoccer {

class Client
{
public:
    Client(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::resolver::results_type &endpoints);

    void connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void setupInputLoop();
    void inputLoop(boost::system::error_code errorCode);

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::posix::stream_descriptor m_desc;

    NCurses m_ncurses;
    Board m_board;
    View m_view;
    std::shared_ptr<Game> m_game;
};

}

#endif // CLIENT_HPP
