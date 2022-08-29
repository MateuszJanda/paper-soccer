// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "View.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "ViewBoardMock.hpp"
#include "ViewMenuMock.hpp"
#include "ViewUtils.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer {

using namespace testing;

namespace {
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
    }

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

    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(0), ViewUtils::vy(0)), std::nullopt);

    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(-1), ViewUtils::vy(-1)), Direction::TopLeft);
    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(-1), ViewUtils::vy(0)), Direction::Left);
    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(-1), ViewUtils::vy(1)), Direction::BottomLeft);
    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(0), ViewUtils::vy(-1)), Direction::Top);
    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(0), ViewUtils::vy(1)), Direction::Bottom);
    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(1), ViewUtils::vy(-1)), Direction::TopRight);
    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(1), ViewUtils::vy(0)), Direction::Right);
    ASSERT_EQ(view.getMoveDirection(ViewUtils::vx(1), ViewUtils::vy(1)), Direction::BottomRight);
}

} // namespace PaperSoccer
