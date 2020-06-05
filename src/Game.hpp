#ifndef GAME_HPP
#define GAME_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "View.hpp"
#include "TmpMoveMsg.hpp"
#include <boost/asio.hpp>

namespace PaperSoccer {

class Game {
public:
    Game(IBoard& board, INCurses& ncurses, View &view, boost::asio::ip::tcp::socket socket);

//    void run();
//    void run1();
//    void run2(boost::asio::io_context& io_context);
    void on_input();
    void makeMove(int d, int x, int y);
    void readMsg();
    void send();
    Direction keyToDirection(int c);

private:
    IBoard& m_board;
    INCurses& m_ncurses;
    View& m_view;
    boost::asio::ip::tcp::socket m_socket;

    std::shared_ptr<boost::asio::posix::stream_descriptor> dxx;
    std::function<void(boost::system::error_code)> input_loop;

    TmpMoveMsg msg;
};

}

#endif // GAME_HPP
