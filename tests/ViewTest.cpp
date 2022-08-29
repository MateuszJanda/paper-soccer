// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "View.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "ViewMenuMock.hpp"
#include "ViewBoardMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer {

using namespace testing;

namespace {
    // const Skip EMPTY;
    // const Skip ALL{Direction::Top, Direction::TopRight, Direction::Right};
    // const Skip TOP_TOPRIGHT{Direction::Top, Direction::TopRight};
    // const Skip TOPRIGHT{Direction::TopRight};
    // const Skip TOPLEFT{Direction::TopLeft};
    // const Skip TOPRIGHT_RIGHT{Direction::TopRight, Direction::Right};
    // const Skip TOP_RIGHT{Direction::Top, Direction::Right};
    // const Skip RIGHT{Direction::Right};

    // constexpr MarkerVisability INVISIBLE{MarkerVisability::Invisible};
    // constexpr MarkerVisability OCCUPIED{MarkerVisability::Occupied};
    // constexpr MarkerVisability NOT_OCCUPIED{MarkerVisability::NotOccupied};

    // constexpr std::size_t WIDTH{9};
    // constexpr std::size_t HEIGHT{13};

    // constexpr int TOP_NET_LINE{0};
    // constexpr int BOTTOM_NET_LINE{HEIGHT - 1};
    // constexpr int TOP_BORDER_LINE{TOP_NET_LINE + 1};
    // constexpr int CENTER_LINE{6};
    // constexpr int RIGHT_LINE{WIDTH - 1};
    // constexpr int GOALPOST_LEFT{3};
    // constexpr int GOALPOST_RIGHT{5};

    // const Position NODE_POS{0, 0};
    // const Position NEIGHBOUR_POS{NODE_POS.x + 1, NODE_POS.y};

    const std::string TOP_NAME{"TOP_NAME"};
    const std::string BOTTOM_NAME{"BOTTOM_NAME"};

    const std::chrono::milliseconds USER_TIME_LEFT{61};
    const std::chrono::milliseconds ENEMY_TIME_LEFT{122};
} // namespace anonymous

class ViewTest : public testing::Test {
public:
    ViewTest()
        : view(boardMock, ncursesMock, viewBoardMock, viewMenuMock)
    {
        // EXPECT_CALL(boardMock, getGoalpostLeft()).WillRepeatedly(Return(GOALPOST_LEFT));
        // EXPECT_CALL(boardMock, getGoalpostRight()).WillRepeatedly(Return(GOALPOST_RIGHT));
        // EXPECT_CALL(boardMock, getHeight()).WillRepeatedly(Return(HEIGHT));
        // EXPECT_CALL(boardMock, getWidth()).WillRepeatedly(Return(WIDTH));
    }

    int vx(int x)
    {
        return x * View::X_FACTOR + View::X_OFFSET;
    }

    int vy(int y)
    {
        return y * View::Y_FACTOR + View::Y_OFFSET;
    }

    // void expectClearLines(Position nodePos = NODE_POS)
    // {
    //     auto x = vx(nodePos.x);
    //     auto y = vy(nodePos.y) - 1;
    //     EXPECT_CALL(ncursesMock, print(x, y, " ", _));

    //     x = vx(nodePos.x) + 1;
    //     y = vy(nodePos.y);
    //     EXPECT_CALL(ncursesMock, print(x, y, "  ", _));

    //     x = vx(nodePos.x) + 1;
    //     y = vy(nodePos.y) - 1;
    //     EXPECT_CALL(ncursesMock, print(x, y, "  ", _));
    // }

    // void expectDrawScore()
    // {
    //     const auto y = View::Y_SCORE_OFFSET;
    //     EXPECT_CALL(ncursesMock, print(_, y + 0, _, _));
    //     EXPECT_CALL(ncursesMock, print(_, y + 1, _, _));
    //     EXPECT_CALL(ncursesMock, print(_, y + 2, _, _));
    // }

    // void expectDrawStatus()
    // {
    //     EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 0, _, _));
    //     EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 1, _, _));
    //     EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 2, _, _));
    //     EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 3, _, _));
    //     EXPECT_CALL(ncursesMock, refreshView());
    // }

    StrictMock<BoardMock> boardMock;
    StrictMock<NCursesMock> ncursesMock;
    StrictMock<ViewBoardMock> viewBoardMock;
    StrictMock<ViewMenuMock> viewMenuMock;
    View view;
};

TEST_F(ViewTest, checkClear)
{
    EXPECT_CALL(ncursesMock, clearView());
    view.clear();
}

// TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenNotThisLine)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{0, TOP_BORDER_LINE});
//     ASSERT_EQ(nodeSkip, EMPTY);
//     ASSERT_EQ(neighSkip, EMPTY);
// }

// TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenCorner)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{0, TOP_NET_LINE});
//     ASSERT_EQ(nodeSkip, ALL);
//     ASSERT_EQ(neighSkip, TOPLEFT);
// }

// TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenGoalpostLeft)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{GOALPOST_LEFT, TOP_NET_LINE});
//     ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
//     ASSERT_EQ(neighSkip, TOPLEFT);
// }

// TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotThisLine)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{0, TOP_BORDER_LINE});
//     ASSERT_EQ(nodeSkip, EMPTY);
//     ASSERT_EQ(neighSkip, EMPTY);
// }

// TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotAGoal)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{0, BOTTOM_NET_LINE});
//     ASSERT_EQ(nodeSkip, ALL);
//     ASSERT_EQ(neighSkip, TOPLEFT);
// }

// TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenInTheMiddleOfGoal)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{GOALPOST_LEFT + 1, BOTTOM_NET_LINE});
//     ASSERT_EQ(nodeSkip, EMPTY);
//     ASSERT_EQ(neighSkip, EMPTY);
// }

// TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenGoalpostRight)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{GOALPOST_RIGHT, BOTTOM_NET_LINE});
//     ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
//     ASSERT_EQ(neighSkip, TOPLEFT);
// }

// TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenNotThisLine)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{0, TOP_NET_LINE});
//     ASSERT_EQ(nodeSkip, EMPTY);
//     ASSERT_EQ(neighSkip, EMPTY);
// }

// TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenCorner)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{0, TOP_BORDER_LINE});
//     ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
//     ASSERT_EQ(neighSkip, TOPLEFT);
// }

// TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostLeft)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{GOALPOST_LEFT, TOP_BORDER_LINE});
//     ASSERT_EQ(nodeSkip, EMPTY);
//     ASSERT_EQ(neighSkip, EMPTY);
// }

// TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostRight)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{GOALPOST_RIGHT, TOP_BORDER_LINE});
//     ASSERT_EQ(nodeSkip, TOPRIGHT);
//     ASSERT_EQ(neighSkip, TOPLEFT);
// }

// TEST_F(ViewTest, checkFilterDirsForRightLineWhenNotThisLine)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForRightLine(Position{0, CENTER_LINE});
//     ASSERT_EQ(nodeSkip, EMPTY);
//     ASSERT_EQ(neighSkip, EMPTY);
// }

// TEST_F(ViewTest, checkFilterDirsForRightLineWhenThisLine)
// {
//     const auto [nodeSkip, neighSkip] = view.filterDirsForRightLine(Position{RIGHT_LINE, CENTER_LINE});
//     ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
//     ASSERT_EQ(neighSkip, EMPTY);
// }

// TEST_F(ViewTest, checkMarkerVisabilityWhenFirstLineAndDoesNotHaveAnyNeighbours)
// {
//     Position nodePos{0, 0};
//     EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(false));
//     EXPECT_CALL(boardMock, hasAnyNeighbour(nodePos)).WillOnce(Return(false));

//     ASSERT_EQ(view.markerVisability(nodePos), MarkerVisability::NotOccupied);
// }

// TEST_F(ViewTest, checkMarkerVisabilityWhenLastLineAndDoesNotHaveAnyNeighbours)
// {
//     Position nodePos{HEIGHT - 1, 0u};
//     EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(false));
//     EXPECT_CALL(boardMock, hasAnyNeighbour(nodePos)).WillOnce(Return(false));

//     ASSERT_EQ(view.markerVisability(nodePos), MarkerVisability::NotOccupied);
// }

// TEST_F(ViewTest, checkMarkerVisabilityWhenFirstLineAndHasAllNeighbours)
// {
//     Position nodePos{0, 0};
//     EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(true));

//     ASSERT_EQ(view.markerVisability(nodePos), MarkerVisability::Invisible);
// }

// TEST_F(ViewTest, checkMarkerVisabilityWhenLastLineAndHasAllNeighbours)
// {
//     Position nodePos{HEIGHT - 1, 0u};
//     EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(true));

//     ASSERT_EQ(view.markerVisability(nodePos), MarkerVisability::Invisible);
// }

// TEST_F(ViewTest, checkMarkerVisabilityWhenHasAnyNeighbours)
// {
//     Position nodePos{0, 0};
//     EXPECT_CALL(boardMock, hasAllNeighbours(nodePos)).WillOnce(Return(false));
//     EXPECT_CALL(boardMock, hasAnyNeighbour(nodePos)).WillOnce(Return(true));

//     ASSERT_EQ(view.markerVisability(nodePos), MarkerVisability::Occupied);
// }

// TEST_F(ViewTest, checkDrawCellPlusMarkerSkipAllDirs)
// {
//     expectClearLines();

//     auto x = vx(NODE_POS.x);
//     auto y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, ALL, TOPLEFT, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellNoMarkerSkipAllDirs)
// {
//     expectClearLines();

//     view.drawCell(NODE_POS, ALL, TOPLEFT, INVISIBLE);
// }

// TEST_F(ViewTest, checkDrawCellNotOccupiedMarkerSkipAllDirs)
// {
//     expectClearLines();

//     auto x = vx(NODE_POS.x);
//     auto y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", ColorPair::MARK_GRAY));

//     view.drawCell(NODE_POS, ALL, TOPLEFT, NOT_OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellPlusMarkerNoNeighbour)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Top)).WillOnce(Return(false));

//     auto x = vx(NODE_POS.x);
//     auto y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, TOPRIGHT_RIGHT, TOPLEFT, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellTopPath)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Top)).WillOnce(Return(true));

//     auto x = vx(NODE_POS.x);
//     auto y = vy(NODE_POS.y) - 1;
//     EXPECT_CALL(ncursesMock, print(x, y, "|", _));

//     x = vx(NODE_POS.x);
//     y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, TOPRIGHT_RIGHT, TOPLEFT, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellRightPathNoNeighbour)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Right)).WillOnce(Return(false));

//     auto x = vx(NODE_POS.x);
//     auto y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, TOP_TOPRIGHT, TOPLEFT, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellRightPath)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Right)).WillOnce(Return(true));

//     auto x = vx(NODE_POS.x) + 1;
//     auto y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "--", _));

//     x = vx(NODE_POS.x);
//     y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, TOP_TOPRIGHT, TOPLEFT, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellTopRightPathNoNeighbour)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(false));

//     auto x = vx(NODE_POS.x);
//     auto y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, TOP_RIGHT, TOPLEFT, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellTopRightPath)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(true));

//     auto x = vx(NODE_POS.x) + 1;
//     auto y = vy(NODE_POS.y) - 1;
//     EXPECT_CALL(ncursesMock, print(x, y, "⸝⸍", _));

//     x = vx(NODE_POS.x);
//     y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, TOP_RIGHT, TOPLEFT, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellTopLeftPathNeighbourOutOfRange)
// {
//     Position nodePos{WIDTH - 1, 0u};
//     expectClearLines(nodePos);

//     auto x = vx(nodePos.x);
//     auto y = vy(nodePos.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(nodePos, ALL, EMPTY, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellTopLeftPathNoNeighbour)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(false));

//     auto x = vx(NODE_POS.x);
//     auto y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, ALL, EMPTY, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellTopLeftPath)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(true));

//     auto x = vx(NEIGHBOUR_POS.x) - 2;
//     auto y = vy(NEIGHBOUR_POS.y) - 1;
//     EXPECT_CALL(ncursesMock, print(x, y, "⸌⸜", _));

//     x = vx(NODE_POS.x);
//     y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, ALL, EMPTY, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawCellCrossPath)
// {
//     expectClearLines();

//     EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(true));
//     EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(true));

//     auto x = vx(NODE_POS.x) + 1;
//     auto y = vy(NODE_POS.y) - 1;
//     EXPECT_CALL(ncursesMock, print(x, y, "ᐳᐸ", _));

//     x = vx(NODE_POS.x);
//     y = vy(NODE_POS.y);
//     EXPECT_CALL(ncursesMock, print(x, y, "+", _));

//     view.drawCell(NODE_POS, TOP_RIGHT, EMPTY, OCCUPIED);
// }

// TEST_F(ViewTest, checkDrawNames)
// {
//     EXPECT_CALL(ncursesMock, print(_, _, TOP_NAME, _));
//     EXPECT_CALL(ncursesMock, print(_, _, BOTTOM_NAME, _));

//     view.drawNames(TOP_NAME, ColorPair::DEFAULT, BOTTOM_NAME, ColorPair::DEFAULT);
// }

// TEST_F(ViewTest, checkDrawPathMarkers)
// {
//     const Position ballPos{0, 0};
//     EXPECT_CALL(boardMock, getBallPosition()).WillOnce(Return(ballPos));
//     EXPECT_CALL(ncursesMock, print(_, _, "*", _)).Times(2);

//     const std::vector<Direction> dirPath{Direction::Left};
//     view.drawPathMarkers(dirPath, ColorPair::DEFAULT);
// }

TEST_F(ViewTest, checkDrawLegend)
{
    const std::map<char, Direction> dirKeys{{'q', Direction::TopLeft}};
    EXPECT_CALL(viewMenuMock, drawLegend('z', 'n', dirKeys));
    view.drawLegend('z', 'n', dirKeys);
}

TEST_F(ViewTest, checkSetContinueStatus)
{
    EXPECT_CALL(viewMenuMock, setContinueStatus());
    view.setContinueStatus();
}

TEST_F(ViewTest, checkDrawBoard)
{
    const std::vector<Direction> dirPath{Direction::Left};

    EXPECT_CALL(viewBoardMock, drawBoard(TOP_NAME, ColorPair::DEFAULT, BOTTOM_NAME, ColorPair::DEFAULT, dirPath, ColorPair::DEFAULT));
    view.drawBoard(TOP_NAME, ColorPair::DEFAULT, BOTTOM_NAME, ColorPair::DEFAULT, dirPath, ColorPair::DEFAULT);
}

TEST_F(ViewTest, checkSetEnemyTurnStatus)
{
    EXPECT_CALL(viewMenuMock, setEnemyTurnStatus());
    view.setEnemyTurnStatus();
}

TEST_F(ViewTest, checkSetReadyToEndTurnStatus)
{
    EXPECT_CALL(viewMenuMock, setReadyToEndTurnStatus());
    view.setReadyToEndTurnStatus();
}

TEST_F(ViewTest, checkSetLostStatus)
{
    EXPECT_CALL(viewMenuMock, setLostStatus(0, 1));
    view.setLostStatus(0, 1);
}

TEST_F(ViewTest, checkSetWinStatus)
{
    EXPECT_CALL(viewMenuMock, setWinStatus(1, 0));
    view.setWinStatus(1, 0);
}

TEST_F(ViewTest, checkDrawTimeLeft)
{
    EXPECT_CALL(viewMenuMock, drawTimeLeft(USER_TIME_LEFT, ENEMY_TIME_LEFT));
    view.drawTimeLeft(USER_TIME_LEFT, ENEMY_TIME_LEFT);
}

TEST_F(ViewTest, checkDrawUserTimeLeft)
{
    EXPECT_CALL(viewMenuMock, drawUserTimeLeft(USER_TIME_LEFT));
    view.drawUserTimeLeft(USER_TIME_LEFT);
}

TEST_F(ViewTest, checkDrawEnemyTimeLeft)
{
    EXPECT_CALL(viewMenuMock, drawEnemyTimeLeft(ENEMY_TIME_LEFT));
    view.drawEnemyTimeLeft(ENEMY_TIME_LEFT);
}

TEST_F(ViewTest, checkIsStatusButtonNotClicked)
{
    EXPECT_CALL(viewMenuMock, isStatusButton(0, 0)).WillOnce(Return(false));

    ASSERT_FALSE(view.isStatusButton(0, 0));
}

TEST_F(ViewTest, checkIsStatusButtonClicked)
{
    EXPECT_CALL(viewMenuMock, isStatusButton(1, 3)).WillOnce(Return(true));
    ASSERT_TRUE(view.isStatusButton(1, 3));
}

TEST_F(ViewTest, checkGetMouseDirection)
{
    const Position ballPos{0, 0};
    EXPECT_CALL(boardMock, getBallPosition()).WillRepeatedly(Return(ballPos));

    ASSERT_EQ(view.getMoveDirection(vx(0), vy(0)), std::nullopt);

    ASSERT_EQ(view.getMoveDirection(vx(-1), vy(-1)), Direction::TopLeft);
    ASSERT_EQ(view.getMoveDirection(vx(-1), vy(0)), Direction::Left);
    ASSERT_EQ(view.getMoveDirection(vx(-1), vy(1)), Direction::BottomLeft);
    ASSERT_EQ(view.getMoveDirection(vx(0), vy(-1)), Direction::Top);
    ASSERT_EQ(view.getMoveDirection(vx(0), vy(1)), Direction::Bottom);
    ASSERT_EQ(view.getMoveDirection(vx(1), vy(-1)), Direction::TopRight);
    ASSERT_EQ(view.getMoveDirection(vx(1), vy(0)), Direction::Right);
    ASSERT_EQ(view.getMoveDirection(vx(1), vy(1)), Direction::BottomRight);
}

} // namespace PaperSoccer
