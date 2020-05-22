#pragma once

#include "Direction.hpp"
#include "Position.hpp"
#include <set>
#include <vector>

namespace PaperSoccer {

class Node {
public:
    bool addNeighbour(Direction dir);
    void addNeighbours(std::initializer_list<Direction> dirList);
    bool delNeighbour(Direction dir);
    bool hasNeighbour(Direction dir) const;
    unsigned degree() const;
    bool canEnter() const;
    bool isLonely() const;
    std::vector<Position> neighboursPositions(Position currentPos) const;

private:
    std::set<Direction> m_neighbours;
};

} // namespace PaperSoccer
