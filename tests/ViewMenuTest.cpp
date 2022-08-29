// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "ViewMenu.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "ViewUtils.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer {

using namespace testing;

namespace {
    constexpr std::size_t WIDTH{9};

    const std::chrono::seconds USER_TIME_LEFT{61};
    const std::chrono::seconds ENEMY_TIME_LEFT{122};
} // namespace anonymous

class ViewMenuTest : public testing::Test {
public:
    ViewMenuTest()
        : viewMenu(boardMock, ncursesMock)
    {
        EXPECT_CALL(boardMock, getWidth()).WillRepeatedly(Return(WIDTH));
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
    ViewMenu viewMenu;
};

TEST_F(ViewMenuTest, checkDrawLegend)
{
    EXPECT_CALL(ncursesMock, print(_, _, _, _)).Times(AtLeast(1));

    const std::map<char, Direction> dirKeys{{'q', Direction::TopLeft}};
    viewMenu.drawLegend('z', 'n', dirKeys);
}

TEST_F(ViewMenuTest, checkSetContinueStatus)
{
    expectDrawStatus();
    viewMenu.setContinueStatus();
}

TEST_F(ViewMenuTest, checkSetEnemyTurnStatus)
{
    expectDrawStatus();
    viewMenu.setEnemyTurnStatus();
}

TEST_F(ViewMenuTest, checkSetReadyToEndTurnStatus)
{
    expectDrawStatus();
    viewMenu.setReadyToEndTurnStatus();
}

TEST_F(ViewMenuTest, checkSetLostStatus)
{
    expectDrawStatus();
    expectDrawScore();
    viewMenu.setLostStatus(0, 1);
}

TEST_F(ViewMenuTest, checkSetWinStatus)
{
    expectDrawStatus();
    expectDrawScore();
    viewMenu.setWinStatus(1, 0);
}

TEST_F(ViewMenuTest, checkDrawTimeLeft)
{
    EXPECT_CALL(ncursesMock, print(_, _, " Time left:", _));
    EXPECT_CALL(ncursesMock, print(_, _, "     Me: 01:01", _));
    EXPECT_CALL(ncursesMock, print(_, _, "  Enemy: 02:02", _));
    EXPECT_CALL(ncursesMock, refreshView()).Times(2);

    viewMenu.drawTimeLeft(USER_TIME_LEFT, ENEMY_TIME_LEFT);
}

TEST_F(ViewMenuTest, checkDrawUserTimeLeft)
{
    EXPECT_CALL(ncursesMock, print(_, _, "     Me: 01:01", _));
    EXPECT_CALL(ncursesMock, refreshView());

    viewMenu.drawUserTimeLeft(USER_TIME_LEFT);
}

TEST_F(ViewMenuTest, checkDrawEnemyTimeLeft)
{
    EXPECT_CALL(ncursesMock, print(_, _, "  Enemy: 02:02", _));
    EXPECT_CALL(ncursesMock, refreshView());

    viewMenu.drawEnemyTimeLeft(ENEMY_TIME_LEFT);
}

TEST_F(ViewMenuTest, checkIsStatusButtonNotClicked)
{
    ASSERT_FALSE(viewMenu.isStatusButton(0, 0));
}

TEST_F(ViewMenuTest, checkIsStatusButtonClicked)
{
    const auto x = ViewUtils::vx(WIDTH) + 2;
    const auto y = ViewUtils::Y_OFFSET;
    ASSERT_TRUE(viewMenu.isStatusButton(x, y));
}

} // namespace PaperSoccer
