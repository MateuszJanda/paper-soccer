#include "View.hpp"
#include "BoardMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer
{

namespace {
const std::set<Direction> EMPTY;
const std::set<Direction> ALL{Direction::Top, Direction::TopRight, Direction::Right};
const std::set<Direction> TTRIGHT{Direction::Top, Direction::TopRight};
const std::set<Direction> RTRIGHT{Direction::TopRight, Direction::Right};

const std::size_t FIRST_LINE{0};
const std::size_t LAST_LINE{12};
const std::size_t GOALPOST_LEFT{3};
const std::size_t GOALPOST_RIGHT{5};
}

using namespace testing;

class ViewTest : public testing::Test {
public:
    ViewTest() : view(boardMock)
    {
        EXPECT_CALL(boardMock, getGoalpostLeft()).WillRepeatedly(Return(GOALPOST_LEFT));
        EXPECT_CALL(boardMock, getGoalpostRight()).WillRepeatedly(Return(GOALPOST_RIGHT));
        EXPECT_CALL(boardMock, getHeight()).WillRepeatedly(Return(LAST_LINE+1));
    }
    StrictMock<BoardMock> boardMock;
    View view;
};

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenNotThisLine)
{
    ASSERT_EQ(view.filterDirsForTopBorderLine(Position{0, 1}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenNotAGoal)
{
    ASSERT_EQ(view.filterDirsForTopBorderLine(Position{0, FIRST_LINE}), ALL);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostLeft)
{
    ASSERT_EQ(view.filterDirsForTopBorderLine(Position{GOALPOST_LEFT, FIRST_LINE}), TTRIGHT);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotThisLine)
{
    ASSERT_EQ(view.filterDirsForBottomBorderLine(Position{0, 1}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotAGoal)
{
    ASSERT_EQ(view.filterDirsForBottomBorderLine(Position{0, LAST_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenInTheMiddleOfGoal)
{
    ASSERT_EQ(view.filterDirsForBottomBorderLine(Position{GOALPOST_LEFT+1, LAST_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenGoalpostRight)
{
    ASSERT_EQ(view.filterDirsForBottomBorderLine(Position{GOALPOST_RIGHT, LAST_LINE}), RTRIGHT);
}

}
