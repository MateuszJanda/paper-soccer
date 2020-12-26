// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef NODE_HPP
#define NODE_HPP

#include "DirectionUtils.hpp"
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
    bool isFull() const;
    std::vector<Position> neighboursPositions(const Position& currentPos) const;

private:
    static constexpr std::size_t MAX_NEIGHBOURS{8};
    std::set<Direction> m_neighbours;
};

} // namespace PaperSoccer

#endif // NODE_HPP
