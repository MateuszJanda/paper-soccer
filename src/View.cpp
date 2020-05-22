#include "View.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Position.hpp"
#include <set>

namespace PaperSoccer {

View::View()
{

}

void View::drawBoard(const Board& board)
{
    const std::size_t firstLine = 0;
    const std::size_t lastLine = board.getHeight() - 1;
    const std::size_t firstCol = 0;
    const std::size_t lastCol = board.getWidth() - 1;

    for(std::size_t y = 0; y < board.getHeight(); y++)
    {
        for(std::size_t x = 0; x < board.getWidth(); x++)
        {
            std::set<Direction> skip{};

            if (y == firstLine)
            {
                if (x == board.getGoalpostLeft())
                {
                    std::set<Direction> skip{Direction::Top, Direction::TopRight};
                }
                else if (x == board.getGoalpostRight())
                {
                    std::set<Direction> skip{Direction::Top, Direction::TopRight, Direction::Right};
                }
                else if (x > board.getGoalpostLeft() and x < board.getGoalpostRight())
                {
                    std::set<Direction> skip{Direction::Top, Direction::TopRight};
                }
                else
                {
                    std::set<Direction> skip{Direction::Top, Direction::TopRight, Direction::Right};
                }
            }
            else if (y == lastLine)
            {
                if (x >= board.getGoalpostLeft() and x <= board.getGoalpostRight())
                {

                }
            }
            else if (y == firstLine + 1)
            {
                if (x >= board.getGoalpostLeft() and x <= board.getGoalpostRight())
                {

                }
                else
                {

                }
            }
            else if (y == firstLine - 1)
            {
                if (x >= board.getGoalpostLeft() and x <= board.getGoalpostRight())
                {

                }
                else
                {

                }
            }
            else
            {
                Position nodePos{x, y};
                drawCell(board, nodePos, skip);
            }
        }
    }
}

void View::drawDebugBoard(const Board& board)
{
    std::set<Direction> skip;

    for(std::size_t y = 0; y < board.getHeight(); y++)
    {
        for(std::size_t x = 0; x < board.getWidth(); x++)
        {
            drawCell(board, Position{x, y}, skip);
        }
    }
}

void View::drawCell(const Board& board, Position nodePos, std::set<Direction> skip)
{
    if (not skip.contains(Direction::Top) and board.hasNeighbour(nodePos, Direction::Top))
    {
        drawVerticalToTopLine(nodePos);
    }
    else if (not skip.contains(Direction::Right) and board.hasNeighbour(nodePos, Direction::Right))
    {
        drawHorizontalToRightLine(nodePos);
    }
    else if (not skip.contains(Direction::TopRight) and board.hasNeighbour(nodePos, Direction::TopRight))
    {
        if (nodePos.x + 1 < board.getHeight() and board.hasNeighbour(Position{nodePos.x + 1, nodePos.y}, Direction::TopLeft))
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
