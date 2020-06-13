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

    EXPECT_CALL(networkMock, registerHandlers(_, _, _, _));
    EXPECT_CALL(networkMock, run());

    game.run();
}

TEST_F(GameTest, onInitNewGameWhenFirstTurnIsUser)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(networkMock, sendNewGame(Turn::User, Goal::Bottom));

    game.setFirstTurn(Turn::User);
    game.onInitNewGame();
}

TEST_F(GameTest, onInitNewGameWhenFirstTurnIsEnemy)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setContinueStatus());
    EXPECT_CALL(networkMock, sendNewGame(Turn::Enemy, Goal::Bottom));

    game.setFirstTurn(Turn::Enemy);
    game.onInitNewGame();
}

TEST_F(GameTest, onNewGameWhenUserTurn)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setContinueStatus());

    game.onNewGame(Turn::User, Goal::Bottom);
}

TEST_F(GameTest, onNewGameWhenEnemyTurn)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setEnemyTurnStatus());

    game.onNewGame(Turn::Enemy, Goal::Bottom);
}

TEST_F(GameTest, onKeyboardMouseInputBreakWhenKeyInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(KeyInput{.key='j'})));
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Illegal));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.setCurrentTurn(Turn::User);
    game.onKeyboardMouseInput();
}

TEST_F(GameTest, onKeyboardMouseInputBreakWhenEnterInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.setCurrentTurn(Turn::User);
    game.onKeyboardMouseInput();
}

TEST_F(GameTest, onKeyboardMouseInputBreakWhenMosueInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(MouseInput{.x=2, .y=1})));
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.onKeyboardMouseInput();
}

TEST_F(GameTest, userKeyWhenEnemyTurn)
{
    game.setCurrentTurn(Turn::Enemy);
    game.userKey('j');
}

TEST_F(GameTest, userKeyWhenNoneTurn)
{
    game.setCurrentTurn(Turn::None);
    game.userKey('j');
}


TEST_F(GameTest, userKeyWhenKeyNotKnown)
{
    game.setCurrentTurn(Turn::User);
    game.userKey('0');
}

TEST_F(GameTest, userKeyWhenUserTurnAndProperKeyAndIllegalMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Illegal));
    EXPECT_CALL(viewMock, drawBoard());

    game.setCurrentTurn(Turn::User);
    game.userKey('j');
}

TEST_F(GameTest, userKeyWhenUserTurnAndProperKeyAndContinueMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Continue));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.setCurrentTurn(Turn::User);
    game.userKey('j');
}

TEST_F(GameTest, userKeyWhenUserTurnAndProperKeyAndStopMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Stop));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setReadyToEndTurnStatus());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.setCurrentTurn(Turn::User);
    game.userKey('j');
}

TEST_F(GameTest, userEndTurnWhenNoneTurn)
{
    game.setCurrentTurn(Turn::None);
    game.setUserStatus(MoveStatus::Stop);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenEnemyTurn)
{
    game.setCurrentTurn(Turn::Enemy);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndContinueMove)
{
    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndDeadEndMove)
{
    EXPECT_CALL(viewMock, setLostStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::DeadEnd);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndTopOwnGoal)
{
    EXPECT_CALL(viewMock, setLostStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Top);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndBottomOwnGoal)
{
    EXPECT_CALL(viewMock, setLostStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Bottom);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndTopGoal)
{
    EXPECT_CALL(viewMock, setWinStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Bottom);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndBottomGoal)
{
    EXPECT_CALL(viewMock, setWinStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Top);
    game.userEndTurn();
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndStopMove)
{
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Stop);
    game.userEndTurn();
}

TEST_F(GameTest, onEnemyMoveWhenUserTurn)
{
    game.setCurrentTurn(Turn::User);

    ASSERT_ANY_THROW(game.onEnemyMove(Direction::Top));
}

TEST_F(GameTest, onEnemyMoveWhenEnemyTurnAndIllegalMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Top)).WillOnce(Return(MoveStatus::Illegal));
    EXPECT_CALL(viewMock, drawBoard());

    game.setCurrentTurn(Turn::Enemy);
    ASSERT_ANY_THROW(game.onEnemyMove(Direction::Top));
}

TEST_F(GameTest, onEnemyMoveWhenEnemyTurnAndLegalMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Top)).WillOnce(Return(MoveStatus::Continue));
    EXPECT_CALL(viewMock, drawBoard());

    game.setCurrentTurn(Turn::Enemy);
    game.onEnemyMove(Direction::Top);
}

TEST_F(GameTest, onEnemyMoveWhenEnemyTurnAdnCorrectMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Bottom)).WillOnce(Return(MoveStatus::Continue));
    EXPECT_CALL(viewMock, drawBoard());

    game.setCurrentTurn(Turn::Enemy);
    game.onEnemyMove(Direction::Bottom);
}

} // namespace PaperSoccer
