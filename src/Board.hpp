#pragma once

#include <cstddef>
#include <vector>
#include "Node.hpp"

namespace PaperSoccer {

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
    Board(std::size_t width, std::size_t height);
    std::size_t getWidth() const;
    std::size_t getHeight() const;
    void setBall(Position pos);
    MoveStatus moveBall(Direction dir);

private:
    void setBorders();
    void setBottom();
    bool canReachGoal(Direction dir, int line) const;
    bool isDeadEnd() const;

    Position ballPos;
    std::vector<std::vector<Node>> graph;
};

} // namespace PaperSoccer
