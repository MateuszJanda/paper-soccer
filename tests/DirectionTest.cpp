#include "Direction.hpp"
#include "gtest/gtest.h"

namespace PaperSoccer
{

using namespace testing;

TEST(DirectionTest, checkConversionDirectionTopToPosition)
{
    Position p { 1, 0 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::Top), p);
}

TEST(DirectionTest, checkConversionDirectionTopRightToPosition)
{
    Position p { 2, 0 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::TopRight), p);
}

TEST(DirectionTest, checkConversionDirectionRightToPosition)
{
    Position p { 2, 1 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::Right), p);
}

TEST(DirectionTest, checkConversionDirectionBottomRightToPosition)
{
    Position p { 2, 2 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::BottomRight), p);
}

TEST(DirectionTest, checkConversionDirectionBottomToPosition)
{
    Position p { 1, 2 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::Bottom), p);
}

TEST(DirectionTest, checkConversionDirectionBottomLeftToPosition)
{
    Position p { 0, 2 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::BottomLeft), p);
}

TEST(DirectionTest, checkConversionDirectionLeftToPosition)
{
    Position p { 0, 1 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::Left), p);
}

TEST(DirectionTest, checkConversionDirectionTopLeftToPosition)
{
    Position p { 0, 0 };
    ASSERT_EQ(directionToPosition(Position { 1, 1 }, Direction::TopLeft), p);
}

TEST(DirectionTest, checkReverseDirection)
{
    std::map<Direction, Direction> mapDirs {
        { Direction::Top, Direction::Bottom },
        { Direction::TopRight, Direction::BottomLeft },
        { Direction::Right, Direction::Left },
        { Direction::BottomRight, Direction::TopLeft },
        { Direction::Bottom, Direction::Top },
        { Direction::BottomLeft, Direction::TopRight },
        { Direction::Left, Direction::Right },
        { Direction::TopLeft, Direction::BottomRight },
    };

    for (const auto& [dir1, dir2] : mapDirs) {
        ASSERT_EQ(reverseDirection(dir1), dir2);
    }
}

}
