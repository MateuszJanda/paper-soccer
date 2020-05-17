#pragma once

#include <cstddef>
#include <vector>
#include "Node.hpp"

enum MoveStatus
{
    Illegal,
    Continue,
    Stop,
    DeadEnd,
    TopGoal,
    BottomGoal
};

class Board
{
public:
    Board(unsigned witdth, unsigned height);
    std::size_t getWidth() const;
    std::size_t getHeight() const;
    void setBall(Position pos);
    void moveBall(Direction dir);

private:
    void setBorders();
    void setBottom();
    bool canReachGoal(Direction dir, int line) const;
    bool isDeadEnd() const;

    Position ballPos;
    std::vector<std::vector<Node>> graph;
};
