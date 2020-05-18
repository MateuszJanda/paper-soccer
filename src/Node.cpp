#include "Node.hpp"

namespace Ps {

bool Node::addNeighbour(Direction dir)
{
    if(hasNeighbour(dir))
    {
        return false;
    }
    this->neighbours[dir] = true;
    return true;
}

bool Node::delNeighbour(Direction dir)
{
    if(hasNeighbour(dir))
    {
        return false;
    }
    this->neighbours[dir] = false;
    return true;
}

bool Node::hasNeighbour(Direction dir) const
{
    if (auto it = neighbours.find(dir); it != neighbours.end()) {
        return it->second;
    }

    return false;
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

bool Node::canEnter() const
{
    if(degree() < 7)
    {
        return true;
    }

    return false;
}

bool Node::isLonely() const
{
    return degree() == 0;
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

} // namespace Ps
