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
            auto [nodeSkip, neighSkip] = filterDirsForOutOfBorder(nodePos);
            drawCell(nodePos, nodeSkip, neighSkip);
        }
    }
}

Skips View::filterDirsForOutOfBorder(Position nodePos)
{
    Skip allNodeSkip;
    Skip allNeighSkip;

    Skip nodeSkip;
    Skip neighSkip;

    std::tie(nodeSkip, neighSkip) = filterDirsForTopNetLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    std::tie(nodeSkip, neighSkip) = filterDirsForTopBorderLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    std::tie(nodeSkip, neighSkip) = filterDirsForBottomNetLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    std::tie(nodeSkip, neighSkip) = filterDirsForRightLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    return Skips{allNodeSkip, allNeighSkip};
}

Skips View::filterDirsForTopNetLine(Position nodePos)
{
    const std::size_t topNetLine = 0;

    if (nodePos.y == topNetLine)
    {
        if (nodePos.x >= m_board.getGoalpostLeft() and nodePos.x < m_board.getGoalpostRight())
        {
            return Skips{Skip{Direction::Top, Direction::TopRight}, Skip{Direction::TopLeft}};
        }
        else
        {
            return Skips{Skip{Direction::Top, Direction::TopRight, Direction::Right},
                         Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips View::filterDirsForTopBorderLine(Position nodePos)
{
    const std::size_t topBorderLine = 1;

    if (nodePos.y == topBorderLine)
    {
        if (nodePos.x < m_board.getGoalpostLeft() or nodePos.x > m_board.getGoalpostRight())
        {
            return Skips{Skip{Direction::Top, Direction::TopRight}, Skip{Direction::TopLeft}};
        }
        else if (nodePos.x == m_board.getGoalpostRight())
        {
            return Skips{Skip{Direction::TopRight}, Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips View::filterDirsForBottomNetLine(Position nodePos)
{
    const std::size_t bottomNetLine = m_board.getHeight() - 1;

    if (nodePos.y == bottomNetLine)
    {
        if (nodePos.x < m_board.getGoalpostLeft() or nodePos.x > m_board.getGoalpostRight())
        {
            return Skips{Skip{Direction::Top, Direction::TopRight, Direction::Right},
                         Skip{Direction::TopLeft}};
        }
        else if (nodePos.x == m_board.getGoalpostRight())
        {
            return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips View::filterDirsForRightLine(Position nodePos)
{
    const std::size_t rightLine = m_board.getWidth() - 1;

    if (nodePos.x == rightLine)
    {
        return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{}};
    }

    return Skips{};
}

void View::drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip)
{
    if (not nodeSkip.contains(Direction::Top) and m_board.hasNeighbour(nodePos, Direction::Top))
    {
        drawVerticalToTopLine(nodePos);
    }

    if (not nodeSkip.contains(Direction::Right) and m_board.hasNeighbour(nodePos, Direction::Right))
    {
        drawHorizontalToRightLine(nodePos);
    }

    bool topRight = false;
    if (not nodeSkip.contains(Direction::TopRight) and m_board.hasNeighbour(nodePos, Direction::TopRight))
    {
        topRight = true;
    }

    bool topLeft = false;
    const Position neighbourPos{nodePos.x + 1, nodePos.y};
    if (not neighSkip.contains(Direction::TopLeft) and neighbourPos.x < m_board.getWidth()
        and m_board.hasNeighbour(neighbourPos, Direction::TopLeft))
    {
        topLeft = true;
    }

    if (topRight and topLeft)
    {
        drawCrossToRight(nodePos);
    }
    else if (topRight)
    {
        drawHypotenuseToTopRight(nodePos);
    }
    else if (topLeft)
    {
        drawHypotenuseToTopLeft(neighbourPos);
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

void View::drawHypotenuseToTopLeft(Position nodePos)
{
    m_ncurses.print(nodePos.x * 3 - 1 + X_OFFSET, nodePos.y * 2 - 1 + Y_OFFSET, "\\");
}

void View::drawMarker(Position nodePos)
{
    m_ncurses.print(nodePos.x * 3 + X_OFFSET, nodePos.y * 2 + Y_OFFSET, "+");
}

}
