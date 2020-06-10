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

    EXPECT_CALL(networkMock, registerHandlers(_, _));
    EXPECT_CALL(networkMock, run());

    game.run();
}

TEST_F(GameTest, onKeyboardInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getChar()).WillOnce(Return(std::make_optional(KeyData{.key='j'})));
    EXPECT_CALL(boardMock, moveBall(Direction::Left));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));
    EXPECT_CALL(ncursesMock, getChar()).WillOnce(Return(std::nullopt));

    game.onKeyboardMouseInput();
}

TEST_F(GameTest, onMosueInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getChar()).WillOnce(Return(std::make_optional(MouseData{.x=2, .y=1})));
    EXPECT_CALL(ncursesMock, getChar()).WillOnce(Return(std::nullopt));

    game.onKeyboardMouseInput();
}


TEST_F(GameTest, makeUserMoveButKeyNotKnown)
{
    game.makeUserMove('0');
}

TEST_F(GameTest, makeUserMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.makeUserMove('j');
}


TEST_F(GameTest, onEnemyMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Bottom));
    EXPECT_CALL(viewMock, drawBoard());

    game.onEnemyMove(Direction::Bottom);

}

} // namespace PaperSoccer
