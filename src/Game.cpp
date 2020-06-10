#include "Game.hpp"
#include <TmpMoveMsg.hpp>
#include <sstream>

namespace PaperSoccer {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Game::Game(INetwork& network, IBoard& board, INCurses& ncurses, IView& view)
    : m_network{network}
    , m_board{board}
    , m_ncurses{ncurses}
    , m_view{view}
    , m_keyMap{{'q', Direction::TopLeft},
          {'u', Direction::TopLeft},
          {'i', Direction::Top},
          {'o', Direction::TopRight},
          {'j', Direction::Left},
          {'l', Direction::Right},
          {'m', Direction::BottomLeft},
          {',', Direction::Bottom},
          {'.', Direction::BottomRight}}
{
}

void Game::run()
{
    using namespace std::placeholders;

    m_network.registerHandlers(std::bind(&Game::onKeyboardMouseInput, this),
        std::bind(&Game::onMove, this, _1));
    m_network.run();
}

void Game::makeMove(int key)
{
    if (not m_keyMap.contains(key))
        return;

    Direction dir = m_keyMap.at(key);
    m_board.moveBall(dir);
    m_view.drawBoard();

    TmpMoveMsg msg{dir};
    m_network.send(msg);
}

void Game::onKeyboardMouseInput()
{
    while (true) {
        auto v = m_ncurses.getChar();

        if (not v)
            break;

        std::visit(overloaded {
            [this](const KeyData& data) { makeMove(data.key); },
            [this](const MouseData& data) {  },
        }, *v);

    }
}

void Game::onMove(const TmpMoveMsg& msg)
{
    m_board.moveBall(msg.dir);
    m_view.drawBoard();
}

} // namespace PaperSoccer
