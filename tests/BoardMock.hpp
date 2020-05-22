#pragma once

#include "IBoard.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class BoardMock : public IBoard {
public:
    MOCK_CONST_METHOD0(getWidth, std::size_t());
    MOCK_CONST_METHOD0(getHeight, std::size_t());
    MOCK_CONST_METHOD0(getGoalpostLeft, std::size_t());
    MOCK_CONST_METHOD0(getGoalpostRight, std::size_t());
    MOCK_METHOD1(setBallPosition, void(Position));
    MOCK_CONST_METHOD0(getBallPosition, Position());
    MOCK_CONST_METHOD2(hasNeighbour, bool(Position, Direction));
    MOCK_METHOD1(moveBall, MoveStatus(Direction));
};

} // namespace PaperSoccer
