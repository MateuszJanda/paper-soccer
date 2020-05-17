#pragma once

#include <map>
#include <vector>

enum class Direction
{
    top,
    top_left,
    right,
    bottom_right,
    bottom,
    bottom_left,
    left,
    top_right
};

class Position
{
public:
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
    case Direction::top:       pos += std::make_pair(0, -1); break;
    case Direction::top_right: pos += std::make_pair(1, -1); break;
    case Direction::right:       pos += std::make_pair(1, 0); break;
    case Direction::bottom_right:       pos += std::make_pair(1, 1); break;
    case Direction::bottom:       pos += std::make_pair(0, 1); break;
    case Direction::bottom_left:       pos += std::make_pair(-1, 1); break;
    case Direction::left:       pos += std::make_pair(-1, 0); break;
    case Direction::top_left:       pos += std::make_pair(-1, -1); break;
    default:
        break;
    }

    return pos;
}

Direction reverseDirection(Direction dir)
{
    switch (dir) {
    case Direction::top: return Direction::bottom;
    case Direction::top_right: return Direction::bottom_left;
    case Direction::right: return Direction::left;
    case Direction::bottom_right: return Direction::top_left;
    case Direction::bottom: return Direction::top;
    case Direction::bottom_left: return Direction::top_right;
    case Direction::left: return Direction::right;
    case Direction::top_left: return Direction::bottom_right;
    default:
        break;
    }

    return dir;
}


class Node
{
public:
    Node();

    void addNeighbour(Direction dir);
    void delNeighbour(Direction dir);
    bool hasNeighbour(Direction dir) const;
    unsigned degree() const;
    bool canEnter() const;
    bool isLonely() const;
    std::vector<Positions> neighboursPositions(Position currentPos) const;

private:
    std::map<Direction, bool> neighbours;
};
