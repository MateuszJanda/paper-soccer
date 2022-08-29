// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Board.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer {

namespace {
    constexpr std::size_t HEIGHT{10};
    constexpr std::size_t WIDTH{8};
    constexpr std::size_t GRAPH_HEIGHT{10 + 2 + 1};
    constexpr std::size_t GRAPH_WIDTH{8 + 1};
    constexpr std::size_t GOALPOST_LEFT{3};
    constexpr std::size_t GOALPOST_RIGHT{5};
    constexpr Position TOP_FROM_CENTER{4, 5};
    constexpr Position CENTER{4, 6};
}

class BoardTest : public testing::Test {
public:
    Board board{WIDTH, HEIGHT};
};

TEST_F(BoardTest, checkWidth)
{
    ASSERT_EQ(board.getWidth(), GRAPH_WIDTH);
}

TEST_F(BoardTest, checkHeight)
{
    ASSERT_EQ(board.getHeight(), GRAPH_HEIGHT);
}

TEST_F(BoardTest, checkGoalpostLeft)
{
    ASSERT_EQ(board.getGoalpostLeft(), GOALPOST_LEFT);
}

TEST_F(BoardTest, checkGoalpostRight)
{
    ASSERT_EQ(board.getGoalpostRight(), GOALPOST_RIGHT);
}

TEST_F(BoardTest, checkBallPosition)
{
    ASSERT_EQ(board.getBallPosition(), CENTER);
}

TEST_F(BoardTest, checkReset)
{
    board.moveBall(Direction::Top);
    board.reset();

    ASSERT_EQ(board.getWidth(), GRAPH_WIDTH);
    ASSERT_EQ(board.getHeight(), GRAPH_HEIGHT);
    ASSERT_EQ(board.getBallPosition(), CENTER);

    for (const auto dir : {Direction::Top,
             Direction::TopRight,
             Direction::Right,
             Direction::BottomRight,
             Direction::Bottom,
             Direction::BottomLeft,
             Direction::Left,
             Direction::TopLeft}) {
        ASSERT_FALSE(board.hasNeighbour(CENTER, dir));
    }
}

TEST_F(BoardTest, checkHasNeighbourWhenOutOfRange)
{
    ASSERT_FALSE(board.hasNeighbour(Position{-1, 0}, Direction::Top));
}

TEST_F(BoardTest, checkHasNeighbourWhenThereIsNo)
{
    ASSERT_FALSE(board.hasNeighbour(CENTER, Direction::Top));
}

TEST_F(BoardTest, checkHasNeighbourWhenThereIsOne)
{
    ASSERT_TRUE(board.hasNeighbour(Position{3, 1}, Direction::Top));
}

TEST_F(BoardTest, checkHasAllNeighboursWhenOutOfRange)
{
    ASSERT_FALSE(board.hasAllNeighbours(Position{-1, 0}));
}

TEST_F(BoardTest, checkHasAllNeighboursWhenNotAll)
{
    ASSERT_FALSE(board.hasAllNeighbours(Position{1, 1}));
}

TEST_F(BoardTest, checkHasAllNeighboursWhenAll)
{
    ASSERT_TRUE(board.hasAllNeighbours(Position{0, 0}));
}

TEST_F(BoardTest, checkHasAnyNeighbourWhenOutOfRange)
{
    ASSERT_FALSE(board.hasAnyNeighbour(Position{-1, 0}));
}

TEST_F(BoardTest, checkHasAnyNeighbourWhenNoNeighbours)
{
    ASSERT_FALSE(board.hasAnyNeighbour(Position{1, 2}));
}

TEST_F(BoardTest, checkHasAnyNeighbourWhenSomeNeighbours)
{
    ASSERT_TRUE(board.hasAnyNeighbour(Position{1, 1}));
}

TEST_F(BoardTest, checkSettingBallPositionOutOfRange)
{
    ASSERT_THROW(board.setBallPosition(Position{-1, -1}), std::out_of_range);
}

TEST_F(BoardTest, checkMoveBallAndStop)
{
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Stop);

    Position p{4, 5};
    ASSERT_EQ(board.getBallPosition(), p);
}

TEST_F(BoardTest, checkMoveBallFromGaolAndStop)
{
    board.setBallPosition(Position{5, 12});
    ASSERT_EQ(board.moveBall(Direction::TopLeft), MoveStatus::Stop);
}

TEST_F(BoardTest, checkMoveBallAndIllegalWhenTurnBack)
{
    board.moveBall(Direction::Top);
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Illegal);

    Position p{4, 5};
    ASSERT_EQ(board.getBallPosition(), p);
}

TEST_F(BoardTest, checkMoveBallAndIllegalWhenCorner)
{
    board.setBallPosition(Position{1, 2});
    ASSERT_EQ(board.moveBall(Direction::TopLeft), MoveStatus::Illegal);

    board.setBallPosition(Position{7, 2});
    ASSERT_EQ(board.moveBall(Direction::TopRight), MoveStatus::Illegal);

    board.setBallPosition(Position{1, 10});
    ASSERT_EQ(board.moveBall(Direction::BottomLeft), MoveStatus::Illegal);

    board.setBallPosition(Position{7, 10});
    ASSERT_EQ(board.moveBall(Direction::BottomRight), MoveStatus::Illegal);
}

TEST_F(BoardTest, checkMoveBallAndIllegalOutOfBorder)
{
    board.setBallPosition(Position{0, 6});
    ASSERT_EQ(board.moveBall(Direction::Left), MoveStatus::Illegal);

    board.setBallPosition(Position{8, 6});
    ASSERT_EQ(board.moveBall(Direction::Right), MoveStatus::Illegal);
}

TEST_F(BoardTest, checkMoveBallAndIllegalAlongBorder)
{
    board.setBallPosition(Position{0, 6});
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Illegal);

    board.setBallPosition(Position{0, 6});
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Illegal);

    board.setBallPosition(Position{8, 6});
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Illegal);

    board.setBallPosition(Position{8, 6});
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Illegal);

    board.setBallPosition(Position{5, 1});
    ASSERT_EQ(board.moveBall(Direction::Right), MoveStatus::Illegal);
}

TEST_F(BoardTest, checkMoveBallAndIllegalAlongGoalNet)
{
    board.setBallPosition(Position{3, 11});
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Illegal);
}

TEST_F(BoardTest, checkMoveBallAndTopGoal)
{
    board.setBallPosition(Position{4, 1});
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::TopGoal);

    board.setBallPosition(Position{5, 1});
    ASSERT_EQ(board.moveBall(Direction::TopLeft), MoveStatus::TopGoal);
}

TEST_F(BoardTest, checkMoveBallAndBottomGoal)
{
    board.setBallPosition(Position{4, 11});
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::BottomGoal);
}

TEST_F(BoardTest, checkMoveBallAndDeadEnd)
{
    board.setBallPosition(Position{1, 2});
    ASSERT_EQ(board.moveBall(Direction::Left), MoveStatus::Continue);
    board.setBallPosition(Position{1, 2});
    ASSERT_EQ(board.moveBall(Direction::BottomLeft), MoveStatus::Continue);
    board.setBallPosition(Position{1, 2});
    ASSERT_EQ(board.moveBall(Direction::Bottom), MoveStatus::Stop);

    board.setBallPosition(Position{1, 2});
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Continue);
    board.setBallPosition(Position{1, 2});
    ASSERT_EQ(board.moveBall(Direction::TopRight), MoveStatus::Continue);
    board.setBallPosition(Position{1, 2});
    ASSERT_EQ(board.moveBall(Direction::Right), MoveStatus::Stop);

    board.setBallPosition(Position{2, 3});
    ASSERT_EQ(board.moveBall(Direction::TopLeft), MoveStatus::DeadEnd);
}

TEST_F(BoardTest, checkMoveBallAndContinue)
{
    board.moveBall(Direction::Top);
    board.moveBall(Direction::Left);

    ASSERT_EQ(board.moveBall(Direction::BottomRight), MoveStatus::Continue);
    ASSERT_EQ(board.getBallPosition(), CENTER);
}

TEST_F(BoardTest, checkMoveBallAndBounceOffTheGoalpost)
{
    board.setBallPosition(Position{3, 2});
    ASSERT_EQ(board.moveBall(Direction::Top), MoveStatus::Continue);
}

TEST_F(BoardTest, checkUndoBallMoveWhenDirectionIsIncorrect)
{
    board.moveBall(Direction::Top);
    board.undoBallMove(Direction::Top);

    ASSERT_TRUE(board.hasNeighbour(CENTER, Direction::Top));
    ASSERT_TRUE(board.hasNeighbour(TOP_FROM_CENTER, Direction::Bottom));
    ASSERT_EQ(board.getBallPosition(), TOP_FROM_CENTER);
}

TEST_F(BoardTest, checkUndoBallMoveWhenDirectionIsCorrect)
{
    board.moveBall(Direction::Top);
    board.undoBallMove(Direction::Bottom);

    ASSERT_FALSE(board.hasNeighbour(CENTER, Direction::Top));
    ASSERT_FALSE(board.hasNeighbour(TOP_FROM_CENTER, Direction::Bottom));
    ASSERT_EQ(board.getBallPosition(), CENTER);
}

} // namespace PaperSoccer
