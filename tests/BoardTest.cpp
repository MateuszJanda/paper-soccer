#include "Board.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace PaperSoccer;

namespace {
constexpr std::size_t HEIGHT { 10 };
constexpr std::size_t WIDTH { 8 };
constexpr Position CENTER { 4, 6 };
}

class BoardTest : public testing::Test {
public:
    Board board { WIDTH, HEIGHT };
};

TEST_F(BoardTest, checkSize)
{
    ASSERT_EQ(board.getWidth(), WIDTH);
    ASSERT_EQ(board.getHeight(), HEIGHT);
}

TEST_F(BoardTest, checkBallPosition)
{
    ASSERT_EQ(board.getBallPosition(), CENTER);
}

TEST_F(BoardTest, setBallPositionOutOfRange)
{
    ASSERT_THROW(board.setBallPosition(Position { -1, -1 }), std::out_of_range);
}

TEST_F(BoardTest, moveBallAndStop)
{
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Stop);

    Position p { 4, 5 };
    ASSERT_EQ(board.getBallPosition(), p);
}

TEST_F(BoardTest, moveBallAndIllegalWhenTurnBack)
{
    board.moveBall(Direction::Top);
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Illegal);

    Position p { 4, 5 };
    ASSERT_EQ(board.getBallPosition(), p);
}

TEST_F(BoardTest, moveBallAndIllegalWhenCorner)
{
    board.setBallPosition(Position { 1, 2 });
    ASSERT_EQ(board.moveBall(Direction::TopLeft), MoveStatus::Illegal);

    board.setBallPosition(Position { 7, 2 });
    ASSERT_EQ(board.moveBall(Direction::TopRight), MoveStatus::Illegal);

    board.setBallPosition(Position { 1, 10 });
    ASSERT_EQ(board.moveBall(Direction::BottomLeft), MoveStatus::Illegal);

    board.setBallPosition(Position { 7, 10 });
    ASSERT_EQ(board.moveBall(Direction::BottomRight), MoveStatus::Illegal);
}

TEST_F(BoardTest, moveBallAndIllegalOutOfBorder)
{
    board.setBallPosition(Position { 0, 6 });
    ASSERT_EQ(board.moveBall(Direction::Left), MoveStatus::Illegal);

    board.setBallPosition(Position { 8, 6 });
    ASSERT_EQ(board.moveBall(Direction::Right), MoveStatus::Illegal);
}

TEST_F(BoardTest, moveBallAndIllegalAlongBorder)
{
    board.setBallPosition(Position { 0, 6 });
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Illegal);

    board.setBallPosition(Position { 0, 6 });
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Illegal);

    board.setBallPosition(Position { 8, 6 });
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Illegal);

    board.setBallPosition(Position { 8, 6 });
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Illegal);
}

TEST_F(BoardTest, moveBallAndTopGoal)
{
    board.setBallPosition(Position { 4, 1 });
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::TopGoal);
}

TEST_F(BoardTest, moveBallAndBottomGoal)
{
    board.setBallPosition(Position { 4, 11 });
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::BottomGoal);
}

TEST_F(BoardTest, moveBallAndDeadEnd)
{
    board.setBallPosition(Position { 1, 2 });
    ASSERT_EQ(board.moveBall(Direction::Left), MoveStatus::Continue);
    board.setBallPosition(Position { 1, 2 });
    ASSERT_EQ(board.moveBall(Direction::BottomLeft), MoveStatus::Continue);
    board.setBallPosition(Position { 1, 2 });
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Stop);

    board.setBallPosition(Position { 1, 2 });
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Continue);
    board.setBallPosition(Position { 1, 2 });
    ASSERT_EQ(board.moveBall(Direction::TopRight), MoveStatus::Continue);
    board.setBallPosition(Position { 1, 2 });
    ASSERT_EQ(board.moveBall(Direction::Right), MoveStatus::Stop);

    board.setBallPosition(Position { 2, 3 });
    ASSERT_EQ(board.moveBall(Direction::TopLeft), MoveStatus::DeadEnd);
}

TEST_F(BoardTest, moveBallAndContinue)
{
    board.moveBall(Direction::Top);
    board.moveBall(Direction::Left);

    ASSERT_EQ(board.moveBall(Direction::BottomRight), MoveStatus::Continue);
    ASSERT_EQ(board.getBallPosition(), CENTER);
}

TEST_F(BoardTest, moveBallAndBounceOffTheGoalpost)
{
    board.setBallPosition(Position { 3, 2 });
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Continue);
}
