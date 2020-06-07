#include "Game.hpp"
#include <TmpMoveMsg.hpp>
#include <sstream>

namespace PaperSoccer {

Game::Game(INetwork& network, IBoard& board, INCurses& ncurses, IView& view)
    : m_network{network}
    , m_board{board}
    , m_ncurses{ncurses}
    , m_view{view}
{
}

void Game::run()
{
    using namespace std::placeholders;

    m_network.registerHandlers(std::bind(&Game::handleKeyboardMouseInput, this),
        std::bind(&Game::handleMoveMsg, this, _1));
    m_network.run();
}

Direction Game::keyToDirection(int c)
{
    switch (c) {
    case 'q':
        return Direction::TopLeft;
    case 'w':
        return Direction::Top;
    case 'e':
        return Direction::TopRight;
    case 'a':
        return Direction::Left;
    case 'd':
        return Direction::Right;
    case 'z':
        return Direction::BottomLeft;
    case 'x':
        return Direction::Bottom;
    case 'c':
        return Direction::BottomRight;
    }

    return Direction::Top;
}

void Game::makeMove(int c, int x, int y)
{
    switch (c) {
    case 'q':
        m_board.moveBall(Direction::TopLeft);
        m_view.drawBoard();
        break;
    case 'w':
        m_board.moveBall(Direction::Top);
        m_view.drawBoard();
        break;
    case 'e':
        m_board.moveBall(Direction::TopRight);
        m_view.drawBoard();
        break;
    case 'a':
        m_board.moveBall(Direction::Left);
        m_view.drawBoard();
        break;
    case 'd':
        m_board.moveBall(Direction::Right);
        m_view.drawBoard();
        break;
    case 'z':
        m_board.moveBall(Direction::BottomLeft);
        m_view.drawBoard();
        break;
    case 'x':
        m_board.moveBall(Direction::Bottom);
        m_view.drawBoard();
        break;
    case 'c':
        m_board.moveBall(Direction::BottomRight);
        m_view.drawBoard();
        break;
    }

    if (c == 1000) {
        std::stringstream sss;
        sss << "Mouse " << x << " " << y << "                             ";
        //        m_view.printText(0, 1, sss.str());
    } else if (c != -1) {
        Direction dir = keyToDirection(c);
        TmpMoveMsg msg{dir};
        m_network.send(msg);
    }
}

void Game::handleKeyboardMouseInput()
{
    std::stringstream ss;
    //    ss << "OnLoop: " << std::this_thread::get_id();
    //    m_view.printText(0, 1, ss.str());

    while (1) {
        auto [c, x, y] = m_ncurses.getChar();
        //        m_view.printText(0, 1, std::to_string(c));
        if (c == /*ERR*/ -1)
            break;
        makeMove(c, x, y);
    }
}

void Game::handleMoveMsg(const TmpMoveMsg& msg)
{
    //    m_view.printText(0, 1, "read");
    m_board.moveBall(msg.dir);
    m_view.drawBoard();
}

} // namespace PaperSoccer
