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
    MOCK_METHOD0(reset, void());
    MOCK_CONST_METHOD0(getWidth, std::size_t());
    MOCK_CONST_METHOD0(getHeight, std::size_t());
    MOCK_CONST_METHOD0(getGoalpostLeft, std::size_t());
    MOCK_CONST_METHOD0(getGoalpostRight, std::size_t());
    MOCK_METHOD1(setBallPosition, void(Position));
    MOCK_CONST_METHOD0(getBallPosition, Position());
    MOCK_CONST_METHOD2(hasNeighbour, bool(Position, Direction));
    MOCK_CONST_METHOD1(hasAllNeighbours, bool(Position));
    MOCK_CONST_METHOD1(hasAnyNeighbour, bool(Position));
    MOCK_METHOD1(moveBall, MoveStatus(Direction));
    MOCK_METHOD1(undoBallMove, bool(Direction));
};

} // namespace PaperSoccer

#endif // BOARD_MOCK_HPP
