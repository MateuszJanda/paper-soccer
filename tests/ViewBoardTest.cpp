// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "ViewBoard.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer {

using namespace testing;

namespace {
    const Skip EMPTY;
    const Skip ALL{Direction::Top, Direction::TopRight, Direction::Right};
    const Skip TOP_TOPRIGHT{Direction::Top, Direction::TopRight};
    const Skip TOPRIGHT{Direction::TopRight};
    const Skip TOPLEFT{Direction::TopLeft};
    const Skip TOPRIGHT_RIGHT{Direction::TopRight, Direction::Right};
    const Skip TOP_RIGHT{Direction::Top, Direction::Right};
    const Skip RIGHT{Direction::Right};

    constexpr MarkerVisibility INVISIBLE{MarkerVisibility::Invisible};
    constexpr MarkerVisibility OCCUPIED{MarkerVisibility::Occupied};
    constexpr MarkerVisibility NOT_OCCUPIED{MarkerVisibility::NotOccupied};

    constexpr std::size_t WIDTH{9};
    constexpr std::size_t HEIGHT{13};

    constexpr int TOP_NET_LINE{0};
    constexpr int BOTTOM_NET_LINE{HEIGHT - 1};
    constexpr int TOP_BORDER_LINE{TOP_NET_LINE + 1};
    constexpr int CENTER_LINE{6};
    constexpr int RIGHT_LINE{WIDTH - 1};
    constexpr int GOALPOST_LEFT{3};
    constexpr int GOALPOST_RIGHT{5};

    const Position NODE_POS{0, 0};
    const Position NEIGHBOUR_POS{NODE_POS.x + 1, NODE_POS.y};

    const std::string TOP_NAME{"TOP_NAME"};
    const std::string BOTTOM_NAME{"BOTTOM_NAME"};
} // namespace anonymous

class ViewBoardTest : public testing::Test {
public:
    ViewBoardTest()
        : viewBoard(boardMock, ncursesMock)
    {
        EXPECT_CALL(boardMock, getGoalpostLeft()).WillRepeatedly(Return(GOALPOST_LEFT));
        EXPECT_CALL(boardMock, getGoalpostRight()).WillRepeatedly(Return(GOALPOST_RIGHT));
        EXPECT_CALL(boardMock, getHeight()).WillRepeatedly(Return(HEIGHT));
        EXPECT_CALL(boardMock, getWidth()).WillRepeatedly(Return(WIDTH));
    }

    void expectClearLines(Position nodePos = NODE_POS)
    {
        auto x = ViewUtils::vx(nodePos.x);
        auto y = ViewUtils::vy(nodePos.y) - 1;
        EXPECT_CALL(ncursesMock, print(x, y, " ", _));

        x = ViewUtils::vx(nodePos.x) + 1;
        y = ViewUtils::vy(nodePos.y);
        EXPECT_CALL(ncursesMock, print(x, y, "  ", _));

        x = ViewUtils::vx(nodePos.x) + 1;
        y = ViewUtils::vy(nodePos.y) - 1;
        EXPECT_CALL(ncursesMock, print(x, y, "  ", _));
    }

    void expectDrawScore()
    {
        const auto y = ViewUtils::Y_SCORE_OFFSET;
        EXPECT_CALL(ncursesMock, print(_, y + 0, _, _));
        EXPECT_CALL(ncursesMock, print(_, y + 1, _, _));
        EXPECT_CALL(ncursesMock, print(_, y + 2, _, _));
    }

    void expectDrawStatus()
    {
        EXPECT_CALL(ncursesMock, print(_, ViewUtils::Y_OFFSET + 0, _, _));
        EXPECT_CALL(ncursesMock, print(_, ViewUtils::Y_OFFSET + 1, _, _));
        EXPECT_CALL(ncursesMock, print(_, ViewUtils::Y_OFFSET + 2, _, _));
        EXPECT_CALL(ncursesMock, print(_, ViewUtils::Y_OFFSET + 3, _, _));
        EXPECT_CALL(ncursesMock, refreshView());
    }

    StrictMock<BoardMock> boardMock;
    StrictMock<NCursesMock> ncursesMock;
    ViewBoard viewBoard;
};

TEST_F(ViewBoardTest, checkFilterDirsForTopNetLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForTopNetLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewBoardTest, checkFilterDirsForTopNetLineWhenCorner)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForTopNetLine(Position{0, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, ALL);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewBoardTest, checkFilterDirsForTopNetLineWhenGoalpostLeft)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForTopNetLine(Position{GOALPOST_LEFT, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewBoardTest, checkFilterDirsForBottomBorderLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForBottomNetLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewBoardTest, checkFilterDirsForBottomBorderLineWhenNotAGoal)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForBottomNetLine(Position{0, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, ALL);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewBoardTest, checkFilterDirsForBottomBorderLineWhenInTheMiddleOfGoal)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForBottomNetLine(Position{GOALPOST_LEFT + 1, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewBoardTest, checkFilterDirsForBottomBorderLineWhenGoalpostRight)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForBottomNetLine(Position{GOALPOST_RIGHT, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewBoardTest, checkFilterDirsForTopBorderLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForTopBorderLine(Position{0, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewBoardTest, checkFilterDirsForTopBorderLineWhenCorner)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForTopBorderLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewBoardTest, checkFilterDirsForTopBorderLineWhenGoalpostLeft)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForTopBorderLine(Position{GOALPOST_LEFT, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewBoardTest, checkFilterDirsForTopBorderLineWhenGoalpostRight)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForTopBorderLine(Position{GOALPOST_RIGHT, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewBoardTest, checkFilterDirsForRightLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForRightLine(Position{0, CENTER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewBoardTest, checkFilterDirsForRightLineWhenThisLine)
{
    const auto [nodeSkip, neighSkip] = viewBoard.filterDirsForRightLine(Position{RIGHT_LINE, CENTER_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewBoardTest, checkMarkerVisibilityWhenFirstLineAndDoesNotHaveAnyNeighbours)
{
    Position nodePos{0, 0};
    EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(false));
    EXPECT_CALL(boardMock, hasAnyNeighbour(nodePos)).WillOnce(Return(false));

    ASSERT_EQ(viewBoard.markerVisibility(nodePos), MarkerVisibility::NotOccupied);
}

TEST_F(ViewBoardTest, checkMarkerVisibilityWhenLastLineAndDoesNotHaveAnyNeighbours)
{
    Position nodePos{HEIGHT - 1, 0u};
    EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(false));
    EXPECT_CALL(boardMock, hasAnyNeighbour(nodePos)).WillOnce(Return(false));

    ASSERT_EQ(viewBoard.markerVisibility(nodePos), MarkerVisibility::NotOccupied);
}

TEST_F(ViewBoardTest, checkMarkerVisibilityWhenFirstLineAndHasAllNeighbours)
{
    Position nodePos{0, 0};
    EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(true));

    ASSERT_EQ(viewBoard.markerVisibility(nodePos), MarkerVisibility::Invisible);
}

TEST_F(ViewBoardTest, checkMarkerVisibilityWhenLastLineAndHasAllNeighbours)
{
    Position nodePos{HEIGHT - 1, 0u};
    EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(true));

    ASSERT_EQ(viewBoard.markerVisibility(nodePos), MarkerVisibility::Invisible);
}

TEST_F(ViewBoardTest, checkMarkerVisibilityWhenHasAnyNeighbours)
{
    Position nodePos{0, 0};
    EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(false));
    EXPECT_CALL(boardMock, hasAnyNeighbour(nodePos)).WillOnce(Return(true));

    ASSERT_EQ(viewBoard.markerVisibility(nodePos), MarkerVisibility::Occupied);
}

TEST_F(ViewBoardTest, checkDrawCellPlusMarkerSkipAllDirs)
{
    expectClearLines();

    auto x = ViewUtils::vx(NODE_POS.x);
    auto y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, ALL, TOPLEFT, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellNoMarkerSkipAllDirs)
{
    expectClearLines();

    viewBoard.drawCell(NODE_POS, ALL, TOPLEFT, INVISIBLE);
}

TEST_F(ViewBoardTest, checkDrawCellNotOccupiedMarkerSkipAllDirs)
{
    expectClearLines();

    auto x = ViewUtils::vx(NODE_POS.x);
    auto y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", ColorPair::MARK_GRAY));

    viewBoard.drawCell(NODE_POS, ALL, TOPLEFT, NOT_OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellPlusMarkerNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Top)).WillOnce(Return(false));

    auto x = ViewUtils::vx(NODE_POS.x);
    auto y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, TOPRIGHT_RIGHT, TOPLEFT, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellTopPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Top)).WillOnce(Return(true));

    auto x = ViewUtils::vx(NODE_POS.x);
    auto y = ViewUtils::vy(NODE_POS.y) - 1;
    EXPECT_CALL(ncursesMock, print(x, y, "|", _));

    x = ViewUtils::vx(NODE_POS.x);
    y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, TOPRIGHT_RIGHT, TOPLEFT, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellRightPathNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Right)).WillOnce(Return(false));

    auto x = ViewUtils::vx(NODE_POS.x);
    auto y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, TOP_TOPRIGHT, TOPLEFT, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellRightPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Right)).WillOnce(Return(true));

    auto x = ViewUtils::vx(NODE_POS.x) + 1;
    auto y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "--", _));

    x = ViewUtils::vx(NODE_POS.x);
    y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, TOP_TOPRIGHT, TOPLEFT, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellTopRightPathNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(false));

    auto x = ViewUtils::vx(NODE_POS.x);
    auto y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, TOP_RIGHT, TOPLEFT, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellTopRightPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(true));

    auto x = ViewUtils::vx(NODE_POS.x) + 1;
    auto y = ViewUtils::vy(NODE_POS.y) - 1;
    EXPECT_CALL(ncursesMock, print(x, y, "⸝⸍", _));

    x = ViewUtils::vx(NODE_POS.x);
    y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, TOP_RIGHT, TOPLEFT, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellTopLeftPathNeighbourOutOfRange)
{
    Position nodePos{WIDTH - 1, 0u};
    expectClearLines(nodePos);

    auto x = ViewUtils::vx(nodePos.x);
    auto y = ViewUtils::vy(nodePos.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(nodePos, ALL, EMPTY, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellTopLeftPathNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(false));

    auto x = ViewUtils::vx(NODE_POS.x);
    auto y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, ALL, EMPTY, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellTopLeftPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(true));

    auto x = ViewUtils::vx(NEIGHBOUR_POS.x) - 2;
    auto y = ViewUtils::vy(NEIGHBOUR_POS.y) - 1;
    EXPECT_CALL(ncursesMock, print(x, y, "⸌⸜", _));

    x = ViewUtils::vx(NODE_POS.x);
    y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, ALL, EMPTY, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawCellCrossPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(true));
    EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(true));

    auto x = ViewUtils::vx(NODE_POS.x) + 1;
    auto y = ViewUtils::vy(NODE_POS.y) - 1;
    EXPECT_CALL(ncursesMock, print(x, y, "ᐳᐸ", _));

    x = ViewUtils::vx(NODE_POS.x);
    y = ViewUtils::vy(NODE_POS.y);
    EXPECT_CALL(ncursesMock, print(x, y, "+", _));

    viewBoard.drawCell(NODE_POS, TOP_RIGHT, EMPTY, OCCUPIED);
}

TEST_F(ViewBoardTest, checkDrawNames)
{
    EXPECT_CALL(ncursesMock, print(_, _, TOP_NAME, _));
    EXPECT_CALL(ncursesMock, print(_, _, BOTTOM_NAME, _));

    viewBoard.drawNames(TOP_NAME, ColorPair::DEFAULT, BOTTOM_NAME, ColorPair::DEFAULT);
}

TEST_F(ViewBoardTest, checkDrawPathMarkers)
{
    const Position ballPos{0, 0};
    EXPECT_CALL(boardMock, getBallPosition()).WillOnce(Return(ballPos));
    EXPECT_CALL(ncursesMock, print(_, _, "*", _)).Times(2);

    const std::vector<Direction> dirPath{Direction::Left};
    viewBoard.drawPathMarkers(dirPath, ColorPair::DEFAULT);
}

} // namespace PaperSoccer
