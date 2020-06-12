#include "Game.hpp"
#include "NetworkMock.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "ViewMock.hpp"
#include "gtest/gtest.h"

namespace PaperSoccer {

using namespace testing;

class GameTest : public testing::Test {
public:
    GameTest()
        : game{networkMock, boardMock, ncursesMock, viewMock}
    {
    }

    StrictMock<NetworkMock> networkMock;
    StrictMock<BoardMock> boardMock;
    StrictMock<ViewMock> viewMock;
    StrictMock<NCursesMock> ncursesMock;
    Game game;
};

TEST_F(GameTest, run)
{
    using namespace std::placeholders;

    EXPECT_CALL(networkMock, registerHandlers(_, _, _));
    EXPECT_CALL(networkMock, run());

    game.run();
}

TEST_F(GameTest, onKeyboardKeyInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(KeyInput{.key='j'})));
    EXPECT_CALL(boardMock, moveBall(Direction::Left));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.onKeyboardMouseInput();
}

TEST_F(GameTest, onKeyboardEnterInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(EnterInput{})));
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.onKeyboardMouseInput();
}

TEST_F(GameTest, onMosueInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(MouseInput{.x=2, .y=1})));
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.onKeyboardMouseInput();
}

TEST_F(GameTest, userKeyButKeyNotKnown)
{
    game.userKey('0');
}

TEST_F(GameTest, userKey)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.userKey('j');
}

TEST_F(GameTest, onEnemyMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Bottom));
    EXPECT_CALL(viewMock, drawBoard());

    game.onEnemyMove(Direction::Bottom);
}

} // namespace PaperSoccer
