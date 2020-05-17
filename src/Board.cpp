#include "Board.hpp"
#include <iostream>

namespace
{
constexpr std::size_t X_OFFSET{1};
constexpr std::size_t Y_OFFSET{1+2};
constexpr std::size_t GATE_WIDTH{2};
}

Board::Board(unsigned witdth, unsigned height)
{
    assert(width >= 4);
    assert(width % 2 == 0);
    assert(height >= 4);
    assert(height % 2 == 0);

    // Empty graph
    for(int i = 0; i < height + Y_OFFSET; i++)
    {
        this->graph.push_back(std::vector{witdth + X_OFFSET, Node{}});
    }

    setBorders();
    setBall(Position{(witdth + X_OFFSET)/2, (height + Y_OFFSET)/2});
}

void Board::setBorders()
{
    // Horizontal borders
    for(auto& row: this->graph)
    {
        auto node = row[0];
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::bottom);

        auto rightNode = row[row.size() - 1];
        rightNode.addNeighbour(Direction::top);
        rightNode.addNeighbour(Direction::top_right);
        rightNode.addNeighbour(Direction::right);
        rightNode.addNeighbour(Direction::bottom_right);
        rightNode.addNeighbour(Direction::bottom);
    }

    // Vertical top left borders
    auto row = this->graph[0];
    auto line = this->graph[1];

    const int goalpost = (row.size() / 2) - (GATE_WIDTH / 2);
    for(int i = 0; i < goalpost; i++)
    {
        auto node = line[i];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);

        node = row[i];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
    }

    // Vertical top right borders
    const int g = (row.size() / 2) + (GATE_WIDTH / 2) + 1;
    for(int i = g; i < row.size(); i++)
    {
        auto node = line[i];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);

        node = row[i];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
    }

    // Top goal
    {
        auto node = line[goalpost];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);

        node = row[goalpost];
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);

        node = line[g];
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::right);

        node = raw[g];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
    }

    for(int i = goalpost; i <= g; i++)
    {
        auto node = raw[i];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);
    }

    setBottom();
}


void Board::setBottom()
{
    // Vertical bottom left borders
    auto row = this->graph[this->graph.size() - 1];
    auto line = this->graph[this->graph.size() - 2];

    const int goalpost = (row.size() / 2) - (GATE_WIDTH / 2);
    for(int i = 0; i < goalpost; i++)
    {
        auto node = line[i];
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::left);

        node = row[i];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
    }

    // Vertical bottom right borders
    const int g = (row.size() / 2) + (GATE_WIDTH / 2) + 1;
    for(int i = g; i < row.size(); i++)
    {
        auto node = line[i];
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::left);

        node = row[i];
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
    }

    // Bottom goal
    {
        auto node = line[goalpost];
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::left);

        node = row[goalpost];
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::left);
        node.addNeighbour(Direction::top_left);
        node.addNeighbour(Direction::top);

        node = line[g];
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom;

        node = raw[g];
        node.addNeighbour(Direction::top);
        node.addNeighbour(Direction::top_right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::left);
    }

    for(int i = goalpost; i <= g; i++)
    {
        auto node = raw[i];
        node.addNeighbour(Direction::right);
        node.addNeighbour(Direction::bottom_right);
        node.addNeighbour(Direction::bottom);
        node.addNeighbour(Direction::bottom_left);
        node.addNeighbour(Direction::left);
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

void Board::setBall(Position pos)
{
    this->ballPos = pos;
}

MoveStatus Board::moveBall(Direction dir)
{
    auto newPos = directionToPosition(this->ballPos, dir);

    if(isGoal(newPos, 0))
    {
        this->graph[this->ballPos.y][this->ballPos.x].addNeighbour(dir);
        auto reverse = reverseDirection(dir);
        this->graph[newPos.y][newPos.x].addNeighbour(reverse);
        setBall(newPos);

        return MoveStatus::TopGoal;
    }

    if(isGoal(newPos, this->graph.size() - 1))
    {
        this->graph[this->ballPos.y][this->ballPos.x].addNeighbour(dir);
        auto reverse = reverseDirection(dir);
        this->graph[newPos.y][newPos.x].addNeighbour(reverse);
        setBall(newPos);

        return MoveStatus::BottomGoal;
    }

    if(this->graph[newPos.y][newPos.x].canEnter())
    {
        auto newNode = this->graph[this->ballPos.y][this->ballPos.x];
        bool occupied = newNode.isOccupied();

        this->graph[this->ballPos.y][this->ballPos.x].addNeighbour(dir);
        auto reverse = reverseDirection(dir);
        this->graph[newPos.y][newPos.x].addNeighbour(reverse);
        setBall(newPos);

        if(occupied)
        {
            return MoveStatus::Continue;
        }
        return MoveStatus::Stop;
    }

    return MoveStatus::Illegal;
}

bool Board::isGoal(Position pos, int line) const
{
    auto row = this->graph[line];
    const int goalpost = (row.size() / 2) - (GATE_WIDTH / 2);
    const int g = (row.size() / 2) + (GATE_WIDTH / 2) + 1;

    for (int i = goalpost; i <= g; i++)
    {
        if (pos.y == line and pos.x = i)
        {
            return true;
        }
    }

    return false;
}
