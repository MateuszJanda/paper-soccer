#ifndef GAME_HPP
#define GAME_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "View.hpp"
#include <boost/asio.hpp>

namespace PaperSoccer {

class Game {
public:
    Game(IBoard& board, INCurses& ncurses, View &view);

    void run();
    void run1();
    void run2(boost::asio::io_context& io_context);
    void on_input();
    void ddd(int d, int x, int y);
    void readInitMsg();

private:
    IBoard& m_board;
    INCurses& m_ncurses;
    View& m_view;

    std::shared_ptr<boost::asio::posix::stream_descriptor> dxx;
    std::function<void(boost::system::error_code)> input_loop;
};

}

#endif // GAME_HPP
