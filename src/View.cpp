#include "View.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Position.hpp"

namespace PaperSoccer {

View::View()
{

}

void View::drawBoard(const Board& board)
{
    const std::size_t firstLine = 0;
    const std::size_t lastLine = board.getHeight() - 1;
    const std::size_t firstCol = 0;
    const std::size_t lastLine = board.getWidth() - 1;

    for(std::size_t y = 0; y < board.getHeight(); y++)
    {
        for(std::size_t x = 0; x < board.getWidth(); x++)
        {
            if (y == firstLine)
            {
                if (x >= board.getGoalpostLeft() and x <= board.getGoalpostRight())
                {

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
                drawCell(board, nodePos);
            }
        }
    }
}

void View::drawDebugBoard(const Board& board)
{
    for(std::size_t y = 0; y < board.getHeight(); y++)
    {
        for(std::size_t x = 0; x < board.getWidth(); x++)
        {
            drawCell(board, x, y);
        }
    }
}

void View::drawCell(const Board& board, Position nodePos)
{
    if (board.hasNeighbour(nodePos, Direction::Top))
    {
        drawVerticalToTopLine(nodePos);
    }
    else if (board.hasNeighbour(nodePos, Direction::Right))
    {
        drawHorizontalToRightLine(nodePos);
    }
    else if (board.hasNeighbour(nodePos, Direction::TopRight))
    {
        if (x + 1 < board.getHeight() and board.hasNeighbour(Position{x + 1, y}, Direction::TopLeft))
        {
            drawHypotenuseToTopRight(nodePos);
        }
        else
        {
            drawCrossToRight(nodePos);
        }
    }

    drawMarker(nodePos);
}

}
