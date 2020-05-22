#include "View.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Position.hpp"
#include <set>

namespace PaperSoccer {

View::View(IBoard& board) : m_board{board}
{

}

void View::drawBoard()
{
    for(std::size_t y = 0; y < m_board.getHeight(); y++)
    {
        for(std::size_t x = 0; x < m_board.getWidth(); x++)
        {
            Position nodePos{x, y};
            std::set<Direction> skip = filterDirsForOutOfBorder(nodePos);
            drawCell(nodePos, skip);
        }
    }
}

std::set<Direction> View::filterDirsForOutOfBorder(Position nodePos)
{
    std::set<Direction> skip;

    skip.merge(filterDirsForTopBorderLine(nodePos));
    skip.merge(filterDirsForTopBorderLine(nodePos));


//    else if (y == lastLine)
//    {
//        if (x >= m_board.getGoalpostLeft() and x <= m_board.getGoalpostRight())
//        {

//        }
//    }
//    else if (y == firstLine + 1)
//    {
//        if (x >= m_board.getGoalpostLeft() and x <= m_board.getGoalpostRight())
//        {

//        }
//        else
//        {

//        }
//    }
//    else if (y == firstLine - 1)
//    {
//        if (x >= m_board.getGoalpostLeft() and x <= m_board.getGoalpostRight())
//        {

//        }
//        else
//        {

//        }
//    }
//    else
//    {
//        Position nodePos{x, y};
//        drawCell(m_board, nodePos, skip);
//    }

    return skip;
}

std::set<Direction> View::filterDirsForTopBorderLine(Position nodePos)
{
    const std::size_t firstLine = 0;

    if (nodePos.y == firstLine)
    {
        if (nodePos.x >= m_board.getGoalpostLeft() and nodePos.x < m_board.getGoalpostRight())
        {
            return std::set<Direction>{Direction::Top, Direction::TopRight};
        }
        else
        {
            return std::set<Direction>{Direction::Top, Direction::TopRight, Direction::Right};
        }
    }

    return std::set<Direction>{};
}

std::set<Direction> View::filterDirsForBottomBorderLine(Position nodePos)
{
    const std::size_t lastLine = m_board.getHeight() - 1;

    if (nodePos.y == lastLine)
    {
        if (nodePos.x < m_board.getGoalpostLeft() and nodePos.x > m_board.getGoalpostRight())
        {
            return std::set<Direction>{Direction::Top, Direction::TopRight, Direction::Right};
        }
        else if (nodePos.x == m_board.getGoalpostRight())
        {
            return std::set<Direction>{Direction::TopRight, Direction::Right};
        }
    }

    return std::set<Direction>{};
}

void View::drawCell(Position nodePos, std::set<Direction> skip)
{
    if (not skip.contains(Direction::Top) and m_board.hasNeighbour(nodePos, Direction::Top))
    {
        drawVerticalToTopLine(nodePos);
    }
    else if (not skip.contains(Direction::Right) and m_board.hasNeighbour(nodePos, Direction::Right))
    {
        drawHorizontalToRightLine(nodePos);
    }
    else if (not skip.contains(Direction::TopRight) and m_board.hasNeighbour(nodePos, Direction::TopRight))
    {
        if (nodePos.x + 1 < m_board.getWidth() and m_board.hasNeighbour(Position{nodePos.x + 1, nodePos.y}, Direction::TopLeft))
        {
            drawCrossToRight(nodePos);
        }
        else
        {
            drawHypotenuseToTopRight(nodePos);
        }
    }

    drawMarker(nodePos);
}

void View::drawVerticalToTopLine(Position nodePos)
{

}

void View::drawHorizontalToRightLine(Position nodePos)
{

}

void View::drawCrossToRight(Position nodePos)
{

}

void View::drawHypotenuseToTopRight(Position nodePos)
{

}

void View::drawMarker(Position nodePos)
{

}
}
