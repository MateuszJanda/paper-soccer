// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef BOARD_MOCK_HPP
#define BOARD_MOCK_HPP

#include "IBoard.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class BoardMock : public IBoard {
public:
    MOCK_METHOD(void, reset, ());
    MOCK_METHOD(std::size_t, getWidth, (), (const));
    MOCK_METHOD(std::size_t, getHeight, (), (const));
    MOCK_METHOD(std::size_t, getGoalpostLeft, (), (const));
    MOCK_METHOD(std::size_t, getGoalpostRight, (), (const));
    MOCK_METHOD(void, setBallPosition, (Position));
    MOCK_METHOD(Position, getBallPosition, (), (const));
    MOCK_METHOD(bool, hasNeighbour, (const Position&, Direction), (const));
    MOCK_METHOD(bool, hasAllNeighbours, (const Position&), (const));
    MOCK_METHOD(bool, hasAnyNeighbour, (const Position&), (const));
    MOCK_METHOD(MoveStatus, moveBall, (Direction));
    MOCK_METHOD(bool, undoBallMove, (Direction));
};

} // namespace PaperSoccer

#endif // BOARD_MOCK_HPP
