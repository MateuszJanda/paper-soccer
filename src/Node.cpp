#include "Node.hpp"

Node::Node()
{

}

void Node::addNeighbour(Direction dir)
{
    if(isNeighbour(dir))
    {
        return false;
    }
    this->neighbours[dir] = true;
    return true;
}

void Node::delNeighbour(Direction dir)
{
    if(isNeighbour(dir))
    {
        return false;
    }
    this->neighbours[dir] = false;
    return true;
}

bool Node::isNeighbour(Direction dir) const
{
    return this->neighbours[dir];
}

unsigned Node::degree() const
{
    unsigned counter{0};
    for(auto const& [dir, exist] : this->neighbours)
    {
        if(exist)
        {
            counter++;
        }
    }

    return counter;
}

std::vector<Position> Node::neighboursPositions(Position currentPos) const
{
    std::vector<Position> positions;
    for(auto const& [dir, exist] : this->neighbours)
    {
        if(exist)
        {
            positions.push_back(directionToPosition(currentPos, dir));
        }
    }

    return positions;
}
