#include "View.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Position.hpp"
#include <set>

namespace PaperSoccer {

namespace {
constexpr int X_OFFSET{2};
constexpr int Y_OFFSET{2};

}

View::View(IBoard& board, INCurses &ncurses) : m_board{board}, m_ncurses{ncurses}
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
    skip.merge(filterDirsForBottomBorderLine(nodePos));
    skip.merge(filterDirsForTopGoalLine(nodePos));
    skip.merge(filterDirsForRightLine(nodePos));

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
        if (nodePos.x < m_board.getGoalpostLeft() or nodePos.x > m_board.getGoalpostRight())
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

std::set<Direction> View::filterDirsForTopGoalLine(Position nodePos)
{
    const std::size_t firstGoalLine = 1;

    if (nodePos.y == firstGoalLine)
    {
        if (nodePos.x < m_board.getGoalpostLeft() or nodePos.x > m_board.getGoalpostRight())
        {
            return std::set<Direction>{Direction::Top, Direction::TopRight};
        }
        else if (nodePos.x == m_board.getGoalpostRight())
        {
            return std::set<Direction>{Direction::Right};
        }
    }

    return std::set<Direction>{};
}

std::set<Direction> View::filterDirsForRightLine(Position nodePos)
{
    const std::size_t rightLine = m_board.getWidth() - 1;

    if (nodePos.x == rightLine)
    {
        return std::set<Direction>{Direction::TopRight, Direction::Right};
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
        Position neighbourPos{nodePos.x + 1, nodePos.y};
        if (nodePos.x + 1 < m_board.getWidth() and m_board.hasNeighbour(neighbourPos, Direction::TopLeft))
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
    m_ncurses.print(nodePos.x * 3 + X_OFFSET, nodePos.y * 2 - 1 + Y_OFFSET, "|");
}

void View::drawHorizontalToRightLine(Position nodePos)
{
    m_ncurses.print(nodePos.x * 3 + 1 + X_OFFSET, nodePos.y * 2 + Y_OFFSET, "--");
}

void View::drawCrossToRight(Position nodePos)
{
    m_ncurses.print(nodePos.x * 3 + 1 + X_OFFSET, nodePos.y * 2 - 1 + Y_OFFSET, "><");
}

void View::drawHypotenuseToTopRight(Position nodePos)
{
    m_ncurses.print(nodePos.x * 3 + 1 + X_OFFSET, nodePos.y * 2 - 1 + Y_OFFSET, "/");
}

void View::drawMarker(Position nodePos)
{
    m_ncurses.print(nodePos.x * 3 + X_OFFSET, nodePos.y * 2 + Y_OFFSET, "+");
}

}
