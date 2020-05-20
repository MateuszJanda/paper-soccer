#include "Board.hpp"
#include <iostream>
#include <cassert>


namespace PaperSoccer {

namespace
{
constexpr std::size_t X_OFFSET{1};
constexpr std::size_t Y_OFFSET{1+2};
constexpr std::size_t GATE_WIDTH{2};
}

Board::Board(std::size_t width, std::size_t height)
    : ballPos{static_cast<int>((width + X_OFFSET) / 2), static_cast<int>((height + Y_OFFSET) / 2)},
      goalpostLeft{((width + X_OFFSET) / 2) - (GATE_WIDTH / 2)},
      goalpostRight{((width + X_OFFSET) / 2) + (GATE_WIDTH / 2) + 1}
{
    assert(width >= 4);
    assert(width % 2 == 0);
    assert(height >= 4);
    assert(height % 2 == 0);

    // Empty graph
    for(int i = 0; i < height + Y_OFFSET; i++) {
        this->graph.push_back(std::vector{width + X_OFFSET, Node{}});
    }

    setBorders();
}

void Board::setBorders()
{
    setHorizontalBorders();
    setTopBorders();
    setTopGaol();
    setBottomBorders();
    setBottomGaol();
}

void Board::setHorizontalBorders()
{
    for(auto& row: this->graph) {
        auto& leftNode = row[0];
        leftNode.addNeighbours({Direction::Top, Direction::TopLeft, Direction::Left,
            Direction::BottomLeft, Direction::Bottom});

        auto& rightNode = row[row.size() - 1];
        rightNode.addNeighbours({Direction::Top, Direction::TopRight, Direction::Right,
            Direction::BottomRight, Direction::Bottom});
    }
}

void Board::setTopBorders()
{
    // Left borders
    auto& goalLine = this->graph[0];
    auto& borderLine = this->graph[1];

    for(int i = 0; i < goalpostLeft; i++)
    {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right});

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }

    // Right borders
    for(int i = goalpostRight; i < goalLine.size(); i++)
    {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right});

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }
}

void Board::setTopGaol()
{
    auto& goalLine = this->graph[0];
    auto& borderLine = this->graph[1];

    // Set goal corners
    auto& node1 = borderLine[goalpostLeft];
    node1.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top});

    auto& node2 = goalLine[goalpostLeft];
    node2.addNeighbours({Direction::Bottom, Direction::BottomLeft, Direction::Left,
        Direction::TopLeft, Direction::Top, Direction::TopRight, Direction::Right});

    auto& node3 = borderLine[goalpostRight];
    node3.addNeighbours({Direction::Top, Direction::TopLeft, Direction::Right});

    auto& node4 = goalLine[goalpostRight];
    node4.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
        Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom});

    // Goal net
    for(int i = goalpostLeft; i <= goalpostRight; i++) {
        auto& node = goalLine[i];
        node.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right});
    }
}

void Board::setBottomBorders()
{
    // Left borders
    auto& goalLine = this->graph[this->graph.size() - 1];
    auto& borderLine = this->graph[this->graph.size() - 2];

    for(int i = 0; i < goalpostLeft; i++)
    {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left});

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }

    // Right borders
    for(int i = goalpostRight; i < goalLine.size(); i++)
    {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left});

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }
}

void Board::setBottomGaol()
{
    auto& goalLine = this->graph[0];
    auto& borderLine = this->graph[1];

    auto& node1 = borderLine[goalpostLeft];
    node1.addNeighbours({Direction::Bottom, Direction::BottomLeft, Direction::Left});

    auto& node2 = goalLine[goalpostLeft];
    node2.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
        Direction::BottomLeft, Direction::Left, Direction::TopLeft, Direction::Top});

    auto& node3 = borderLine[goalpostRight];
    node3.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom});

    auto& node4 = goalLine[goalpostRight];
    node4.addNeighbours({Direction::Top, Direction::TopRight, Direction::BottomRight,
        Direction::Bottom, Direction::BottomLeft, Direction::Right, Direction::Left});

    for(int i = goalpostLeft; i <= goalpostRight; i++)
    {
        auto& node = goalLine[i];
        node.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left});
    }
}

std::size_t Board::getWidth() const
{
    return this->graph[0].size() - X_OFFSET;
}

std::size_t Board::getHeight() const
{
    return this->graph.size() - Y_OFFSET;
}

void Board::setBallPosition(Position pos)
{
    if (not isPositionInGraph(pos)) {
        throw std::range_error{"Position out of graph."};
    }
    this->ballPos = pos;
}

Position Board::getBallPosition() const
{
    return this->ballPos;
}

MoveStatus Board::moveBall(Direction dir)
{
    if (this->graph[this->ballPos.y][this->ballPos.x].hasNeighbour(dir))
    {
        return MoveStatus::Illegal;
    }

    auto newPos = directionToPosition(this->ballPos, dir);
    if (not isPositionInGraph(newPos))
    {
        return MoveStatus::Illegal;
    }

    if(canReachGoal(dir, 0))
    {
        this->graph[this->ballPos.y][this->ballPos.x].addNeighbour(dir);
        auto reverse = reverseDirection(dir);
        this->graph[newPos.y][newPos.x].addNeighbour(reverse);
        setBallPosition(newPos);

        return MoveStatus::TopGoal;
    }

    if(canReachGoal(dir, this->graph.size() - 1))
    {
        this->graph[this->ballPos.y][this->ballPos.x].addNeighbour(dir);
        auto reverse = reverseDirection(dir);
        this->graph[newPos.y][newPos.x].addNeighbour(reverse);
        setBallPosition(newPos);

        return MoveStatus::BottomGoal;
    }

    if(this->graph[newPos.y][newPos.x].canEnter())
    {
        auto& newNode = this->graph[this->ballPos.y][this->ballPos.x];
        const bool lonely = newNode.isLonely();

        this->graph[this->ballPos.y][this->ballPos.x].addNeighbour(dir);
        auto reverse = reverseDirection(dir);
        this->graph[newPos.y][newPos.x].addNeighbour(reverse);
        setBallPosition(newPos);

        if (isDeadEnd())
        {
            return MoveStatus::DeadEnd;
        }
        else if(lonely)
        {
            return MoveStatus::Stop;
        }
        return MoveStatus::Continue;
    }

    return MoveStatus::Illegal;
}

bool Board::canReachGoal(Direction dir, int line) const
{
    auto newPos = directionToPosition(this->ballPos, dir);
    for (int x = goalpostLeft; x <= goalpostRight; x++)
    {
        if (newPos.y == line and newPos.x == x)
        {
            return true;
        }
    }

    return false;
}

bool Board::isDeadEnd() const
{
    std::array<Direction, 8> allDirs{    Direction::Top,
                Direction::TopLeft,
                Direction::Right,
                Direction::BottomRight,
                Direction::Bottom,
                Direction::BottomLeft,
                Direction::Left,
                Direction::TopRight};

    for (const auto dir : allDirs)
    {
        auto newPos = directionToPosition(this->ballPos, dir);
        if (not isPositionInGraph(newPos)) {
            continue;
        }

        auto& newNode = this->graph[newPos.y][newPos.x];
        auto& node = this->graph[this->ballPos.y][this->ballPos.x];

        if (not node.hasNeighbour(dir) && newNode.canEnter())
        {
            return false;
        }
    }

    return true;
}

bool Board::isPositionInGraph(Position pos) const
{
    return pos.y >= 0 and pos.y < this->graph.size() and pos.x >= 0 and pos.x < this->graph[0].size();
}

} // namespace PaperSoccer
