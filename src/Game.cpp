#include "Game.hpp"


namespace PaperSoccer {

Game::Game(IBoard& board, INCurses& ncurses, View& view)
  : m_board{board}
  , m_ncurses{ncurses}
  , m_view{view}
{

}

void Game::run()
{
    while(1)
    {
        int c = m_ncurses.getChar();
        switch(c) {
            case 'q':
                m_board.moveBall(Direction::TopLeft);
                m_view.drawBoard();
                break;
        }
    }
}

}
