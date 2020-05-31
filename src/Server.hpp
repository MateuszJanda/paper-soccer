#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include "NCurses.hpp"
#include "View.hpp"
#include "Board.hpp"
#include "Game.hpp"

namespace PaperSoccer {

class Server
{
public:
    Server(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint);

    void accept();
    void setInputLoop();
    void onInput();
    void inputLoop(boost::system::error_code errorCode);

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::posix::stream_descriptor m_desc;

    NCurses m_ncurses;
    Board m_board;
    View m_view;
    std::shared_ptr<Game> m_game;

};


}

#endif // SERVER_HPP
