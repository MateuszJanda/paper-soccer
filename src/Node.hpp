#pragma once

#include <map>
#include <vector>
#include "Position.hpp"
#include "Direction.hpp"


namespace PaperSoccer {

class Node {
public:
    bool addNeighbour(Direction dir);
    bool delNeighbour(Direction dir);
    bool hasNeighbour(Direction dir) const;
    unsigned degree() const;
    bool canEnter() const;
    bool isLonely() const;
    std::vector<Position> neighboursPositions(Position currentPos) const;

private:
    std::map<Direction, bool> neighbours;
};

} // namespace PaperSoccer
