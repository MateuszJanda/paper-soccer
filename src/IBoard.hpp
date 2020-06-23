// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef I_BOARD_HPP
#define I_BOARD_HPP

#include "Node.hpp"
#include <cstddef>

namespace PaperSoccer {

enum MoveStatus {
    Illegal,
    Continue,
    Stop,
    DeadEnd,
    TopGoal,
    BottomGoal
};

class IBoard {
public:
    virtual ~IBoard() noexcept = default;

    virtual void reset() = 0;
    virtual std::size_t getWidth() const = 0;
    virtual std::size_t getHeight() const = 0;
    virtual std::size_t getGoalpostLeft() const = 0;
    virtual std::size_t getGoalpostRight() const = 0;
    virtual void setBallPosition(Position pos) = 0;
    virtual Position getBallPosition() const = 0;
    virtual bool hasNeighbour(Position pos, Direction dir) const = 0;
    virtual bool hasAllNeighbours(Position pos) const = 0;
    virtual bool hasAnyNeighbour(Position pos) const = 0;
    virtual MoveStatus moveBall(Direction dir) = 0;
    virtual bool undoBallMove(Direction dir) = 0;
};

} // namespace PaperSoccer

#endif // I_BOARD_HPP
