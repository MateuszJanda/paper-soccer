#include "View.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer
{

namespace {
const std::set<Direction> EMPTY;
const std::set<Direction> ALL{Direction::Top, Direction::TopRight, Direction::Right};
const std::set<Direction> TTRIGHT{Direction::Top, Direction::TopRight};
const std::set<Direction> TRIGHT{Direction::TopRight};
const std::set<Direction> RTRIGHT{Direction::TopRight, Direction::Right};
const std::set<Direction> RIGHT{Direction::Right};

const std::size_t TOP_NET_LINE{0};
const std::size_t BOTTOM_NET_LINE{12};
const std::size_t TOP_BORDER_LINE{TOP_NET_LINE+1};
const std::size_t BOTTOM_BORDER_LINE{BOTTOM_NET_LINE-1};
const std::size_t CENTER_LINE{6};
const std::size_t RIGHT_LINE{8};
const std::size_t GOALPOST_LEFT{3};
const std::size_t GOALPOST_RIGHT{5};
}

using namespace testing;

class ViewTest : public testing::Test {
public:
    ViewTest() : view(boardMock, ncursesMock)
    {
        EXPECT_CALL(boardMock, getGoalpostLeft()).WillRepeatedly(Return(GOALPOST_LEFT));
        EXPECT_CALL(boardMock, getGoalpostRight()).WillRepeatedly(Return(GOALPOST_RIGHT));
        EXPECT_CALL(boardMock, getHeight()).WillRepeatedly(Return(BOTTOM_NET_LINE+1));
        EXPECT_CALL(boardMock, getWidth()).WillRepeatedly(Return(RIGHT_LINE+1));
    }
    StrictMock<BoardMock> boardMock;
    StrictMock<NCursesMock> ncursesMock;
    View view;
};

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenNotThisLine)
{
    ASSERT_EQ(view.filterDirsForTopNetLine(Position{0, TOP_BORDER_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenCorner)
{
    ASSERT_EQ(view.filterDirsForTopNetLine(Position{0, TOP_NET_LINE}), ALL);
}

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenGoalpostLeft)
{
    ASSERT_EQ(view.filterDirsForTopNetLine(Position{GOALPOST_LEFT, TOP_NET_LINE}), TTRIGHT);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotThisLine)
{
    ASSERT_EQ(view.filterDirsForBottomNetLine(Position{0, TOP_BORDER_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotAGoal)
{
    ASSERT_EQ(view.filterDirsForBottomNetLine(Position{0, BOTTOM_NET_LINE}), ALL);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenInTheMiddleOfGoal)
{
    ASSERT_EQ(view.filterDirsForBottomNetLine(Position{GOALPOST_LEFT+1, BOTTOM_NET_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenGoalpostRight)
{
    ASSERT_EQ(view.filterDirsForBottomNetLine(Position{GOALPOST_RIGHT, BOTTOM_NET_LINE}), RTRIGHT);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenNotThisLine)
{
    ASSERT_EQ(view.filterDirsForTopBorderLine(Position{0, TOP_NET_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenCorner)
{
    ASSERT_EQ(view.filterDirsForTopBorderLine(Position{0, TOP_BORDER_LINE}), TTRIGHT);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostLeft)
{
    ASSERT_EQ(view.filterDirsForTopBorderLine(Position{GOALPOST_LEFT, TOP_BORDER_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostRight)
{
    ASSERT_EQ(view.filterDirsForTopBorderLine(Position{GOALPOST_RIGHT, TOP_BORDER_LINE}), TRIGHT);
}

TEST_F(ViewTest, checkFilterDirsForRightLineWhenNotThisLine)
{
    ASSERT_EQ(view.filterDirsForRightLine(Position{0, CENTER_LINE}), EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForRightLineWhenThisLine)
{
    ASSERT_EQ(view.filterDirsForRightLine(Position{RIGHT_LINE, CENTER_LINE}), RTRIGHT);
}

}
