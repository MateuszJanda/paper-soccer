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
      goalpostLeft{((GATE_WIDTH + X_OFFSET) / 2) - (GATE_WIDTH / 2)},
      goalpostRight{((GATE_WIDTH + X_OFFSET) / 2) + (GATE_WIDTH / 2) + 1}
{
    assert(width >= 4);
    assert(width % 2 == 0);
    assert(height >= 4);
    assert(height % 2 == 0);

    // Empty graph
    for(int i = 0; i < height + Y_OFFSET; i++)
    {
        this->graph.push_back(std::vector{width + X_OFFSET, Node{}});
    }

    setBorders();
}

void Board::setBorders()
{
    setHorizontalBorders();
    setTopBorders();
    setBottomBorders();
}

void Board::setHorizontalBorders()
{
    for(auto& row: this->graph) {
        auto& leftNode = row[0];
        leftNode.addNeighbours({Direction::Top, Direction::TopLeft, Direction::Left,
            Direction::BottomLeft, Direction::Bottom});

        auto rightNode = row[row.size() - 1];
        rightNode.addNeighbours({Direction::Top, Direction::TopRight, Direction::Right,
            Direction::BottomRight, Direction::Bottom});
    }
}

void Board::setTopBorders()
{
    // Left borders
    auto& row = this->graph[0];
    auto& line = this->graph[1];

    for(int i = 0; i < goalpostLeft; i++)
    {
        auto& node = line[i];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);

        node = row[i];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
    }

    // Right borders
    for(int i = goalpostRight; i < row.size(); i++)
    {
        auto& node = line[i];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);

        node = row[i];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
    }

    // Top goal
    {
        auto& node = line[goalpostLeft];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);

        node = row[goalpostLeft];
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);

        node = line[goalpostRight];
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Right);

        node = row[goalpostRight];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
    }

    for(int i = goalpostLeft; i <= goalpostRight; i++)
    {
        auto& node = row[i];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);
    }
}

void Board::setBottomBorders()
{
    // Left borders
    auto& row = this->graph[this->graph.size() - 1];
    auto& line = this->graph[this->graph.size() - 2];

    for(int i = 0; i < goalpostLeft; i++)
    {
        auto& node = line[i];
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
        node.addNeighbour(Direction::Left);

        node = row[i];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
    }

    // Right borders
    for(int i = goalpostRight; i < row.size(); i++)
    {
        auto& node = line[i];
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
        node.addNeighbour(Direction::Left);

        node = row[i];
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
    }

    // Bottom goal
    {
        auto& node = line[goalpostLeft];
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
        node.addNeighbour(Direction::Left);

        node = row[goalpostLeft];
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
        node.addNeighbour(Direction::Left);
        node.addNeighbour(Direction::TopLeft);
        node.addNeighbour(Direction::Top);

        node = line[goalpostRight];
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);

        node = row[goalpostRight];
        node.addNeighbour(Direction::Top);
        node.addNeighbour(Direction::TopRight);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::Left);
    }

    for(int i = goalpostLeft; i <= goalpostRight; i++)
    {
        auto& node = row[i];
        node.addNeighbour(Direction::Right);
        node.addNeighbour(Direction::BottomRight);
        node.addNeighbour(Direction::Bottom);
        node.addNeighbour(Direction::BottomLeft);
        node.addNeighbour(Direction::Left);
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
        auto newNode = this->graph[this->ballPos.y][this->ballPos.x];
        bool lonely = newNode.isLonely();

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
    auto pos = directionToPosition(this->ballPos, dir);

    for (int x = goalpostLeft; x <= goalpostRight; x++)
    {
        if (pos.y == line and pos.x == x)
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

        auto newNode = this->graph[newPos.y][newPos.x];
        auto node = this->graph[this->ballPos.y][this->ballPos.x];

        if (newNode.canEnter() and not node.hasNeighbour(dir))
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
