#include "Node.hpp"

namespace PaperSoccer {

bool Node::addNeighbour(Direction dir)
{
    if (hasNeighbour(dir)) {
        return false;
    }
    m_neighbours.insert(dir);
    return true;
}

void Node::addNeighbours(std::initializer_list<Direction> dirList)
{
    for (const auto& dir : dirList) {
        addNeighbour(dir);
    }
}

bool Node::delNeighbour(Direction dir)
{
    if (not hasNeighbour(dir)) {
        return false;
    }
    m_neighbours.erase(dir);
    return true;
}

bool Node::hasNeighbour(Direction dir) const
{
    return m_neighbours.contains(dir);
}

unsigned Node::degree() const
{
    return m_neighbours.size();
}

bool Node::canEnter() const
{
    if (degree() < 7) {
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
    for (auto const& dir : m_neighbours) {
        positions.push_back(directionToPosition(currentPos, dir));
    }

    return positions;
}

} // namespace PaperSoccer
