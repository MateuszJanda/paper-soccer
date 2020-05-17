#pragma once

#include <map>
#include <vector>

enum class Direction
{
    top_right,
    top,
    top_left,
    right,
    bottom_right,
    bottom,
    bottom_left,
    left
};

class Position
{
    Point(const int x, const int y)
    {
        this->x = x;
        this->y = y;
    }
    int x = -1;
    int y = -1;

    void operator +=(std::pair offset) {
        x += offset.first;
        y += offset.second;
    }
};

Position directionToPosition(Positions pos, Direction dir)
{
    switch (dir) {
    case Direction::top_right: pos += std::make_pair(1, -1); break;
    case Direction::top:       pos += std::make_pair(0, -1); break;
    case Direction::top_left:       pos += std::make_pair(-1, -1); break;
    case Direction::right:       pos += std::make_pair(1, 0); break;
    case Direction::bottom_right:       pos += std::make_pair(1, 1); break;
    case Direction::bottom:       pos += std::make_pair(0, 1); break;
    case Direction::bottom_left:       pos += std::make_pair(-1, 1); break;
    case Direction::left:       pos += std::make_pair(-1, 0); break;
    default:
        break;
    }

    return pos;
}



class Node
{
public:
    Node();

    void addNeighbour(Direction dir);
    void delNeighbour(Direction dir);
    bool isNeighbour(Direction dir) const;
    unsigned degree() const;
    std::vector<Positions> neighboursPositions(Position currentPos) const;

private:
    std::map<Direction, bool> neighbours;
};
