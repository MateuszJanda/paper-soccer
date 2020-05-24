#include "View.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer
{

namespace {
const Skip EMPTY;
const Skip ALL{Direction::Top, Direction::TopRight, Direction::Right};
const Skip TOP_TOPRIGHT{Direction::Top, Direction::TopRight};
const Skip TOPRIGHT{Direction::TopRight};
const Skip TOPLEFT{Direction::TopLeft};
const Skip TOPRIGHT_RIGHT{Direction::TopRight, Direction::Right};
const Skip TOP_RIGHT{Direction::Top, Direction::Right};
const Skip RIGHT{Direction::Right};

constexpr std::size_t WIDTH{9};
constexpr std::size_t HEIGHT{13};

constexpr std::size_t TOP_NET_LINE{0};
constexpr std::size_t BOTTOM_NET_LINE{HEIGHT-1};
constexpr std::size_t TOP_BORDER_LINE{TOP_NET_LINE+1};
constexpr std::size_t BOTTOM_BORDER_LINE{BOTTOM_NET_LINE-1};
constexpr std::size_t CENTER_LINE{6};
constexpr std::size_t RIGHT_LINE{WIDTH-1};
constexpr std::size_t GOALPOST_LEFT{3};
constexpr std::size_t GOALPOST_RIGHT{5};


}

using namespace testing;

class ViewTest : public testing::Test {
public:
    ViewTest() : view(boardMock, ncursesMock)
    {
        EXPECT_CALL(boardMock, getGoalpostLeft()).WillRepeatedly(Return(GOALPOST_LEFT));
        EXPECT_CALL(boardMock, getGoalpostRight()).WillRepeatedly(Return(GOALPOST_RIGHT));
        EXPECT_CALL(boardMock, getHeight()).WillRepeatedly(Return(HEIGHT));
        EXPECT_CALL(boardMock, getWidth()).WillRepeatedly(Return(WIDTH));
    }
    StrictMock<BoardMock> boardMock;
    StrictMock<NCursesMock> ncursesMock;
    View view;
};

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenCorner)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{0, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, ALL);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenGoalpostLeft)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{GOALPOST_LEFT, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotAGoal)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{0, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, ALL);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenInTheMiddleOfGoal)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{GOALPOST_LEFT+1, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenGoalpostRight)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{GOALPOST_RIGHT, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{0, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenCorner)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostLeft)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{GOALPOST_LEFT, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostRight)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{GOALPOST_RIGHT, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForRightLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForRightLine(Position{0, CENTER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForRightLineWhenThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForRightLine(Position{RIGHT_LINE, CENTER_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkDrawCellPlusMarkerSkipAllDirs)
{
    Position p{0,0};
    int x = p.x * 3 + 2;
    int y = p.y * 2 + 2;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, ALL, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellPlusMarkerNoNeighbour)
{
    Position p{0,0};
    EXPECT_CALL(boardMock, hasNeighbour(p, Direction::Top)).WillOnce(Return(false));
    int x = p.x * 3 + 2;
    int y = p.y * 2 + 2;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, TOPRIGHT_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopPath)
{
    Position p{0,0};
    EXPECT_CALL(boardMock, hasNeighbour(p, Direction::Top)).WillOnce(Return(true));
    int x = p.x * 3 + 2;
    int y = p.y * 2 -1 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "|"));
    x = p.x * 3 + 2;
    y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, TOPRIGHT_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellRightPathNoNeighbour)
{
    Position p{0,0};
    EXPECT_CALL(boardMock, hasNeighbour(p, Direction::Right)).WillOnce(Return(false));
    int x = p.x * 3 + 2;
    int y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, TOP_TOPRIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellRightPath)
{
    Position p{0,0};
    EXPECT_CALL(boardMock, hasNeighbour(p, Direction::Right)).WillOnce(Return(true));
    int x = p.x * 3 + 1 + 2;
    int y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "--"));
    x = p.x * 3 + 2;
    y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, TOP_TOPRIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopRightPathNoNeighbour)
{
    Position p{0,0};
    EXPECT_CALL(boardMock, hasNeighbour(p, Direction::TopRight)).WillOnce(Return(false));
    int x = p.x * 3 + 2;
    int y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, TOP_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopRightPath)
{
    Position p{0,0};
    EXPECT_CALL(boardMock, hasNeighbour(p, Direction::TopRight)).WillOnce(Return(true));
    int x = p.x * 3 + 1 + 2;
    int y = p.y * 2 - 1 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "/"));
    x = p.x * 3 + 2;
    y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, TOP_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopLeftPathNeighbourOutOfRange)
{
    Position p{WIDTH-1,0};
    int x = p.x * 3 + 2;
    int y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, ALL, EMPTY);
}

TEST_F(ViewTest, checkDrawCellTopLeftPathNoNeighbour)
{
    Position p{0,0};
    Position p1{1, 0};
    EXPECT_CALL(boardMock, hasNeighbour(p1, Direction::TopLeft)).WillOnce(Return(false));
    int x = p.x * 3 + 2;
    int y = p.y * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, ALL, EMPTY);
}

TEST_F(ViewTest, checkDrawCellTopLeftPath)
{
    Position p{0,0};
    Position p1{1, 0};
    EXPECT_CALL(boardMock, hasNeighbour(p1, Direction::TopLeft)).WillOnce(Return(true));
    int x = p1.x * 3 -1 + 2;
    int y = p1.y * 2 -1 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "\\"));
    x = p.x * 3 + 2;
    y = p.x * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, ALL, EMPTY);
}

TEST_F(ViewTest, checkDrawCellCrossPath)
{
    Position p{0,0};
    EXPECT_CALL(boardMock, hasNeighbour(p, Direction::TopRight)).WillOnce(Return(true));
    Position p1{1, 0};
    EXPECT_CALL(boardMock, hasNeighbour(p1, Direction::TopLeft)).WillOnce(Return(true));
    int x = p.x * 3 + 1 + 2;
    int y = p.y * 2 - 1 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "><"));
    x = p.x * 3 + 2;
    y = p.x * 2 + 2;

    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(p, TOP_RIGHT, EMPTY);
}


}
