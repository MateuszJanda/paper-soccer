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
    }
}

}
