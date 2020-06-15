#include "Game.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "NetworkMock.hpp"
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

    EXPECT_CALL(networkMock, registerHandlers(_, _, _, _, _, _));
    EXPECT_CALL(networkMock, run());

    game.run();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::Connecting);
}

TEST_F(GameTest, initNewGameWhenFirstTurnIsUser)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(networkMock, sendNewGame(Turn::User, Goal::Bottom));

    game.setFirstTurn(Turn::User);
    game.initNewGame();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, initNewGameWhenFirstTurnIsEnemy)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setContinueStatus());
    EXPECT_CALL(networkMock, sendNewGame(Turn::Enemy, Goal::Bottom));

    game.setFirstTurn(Turn::Enemy);
    game.initNewGame();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::User);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, onNewGameWhenUserTurn)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setContinueStatus());

    game.onNewGame(NewGameMsg{Turn::User, Goal::Bottom});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::User);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, onNewGameWhenEnemyTurn)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setEnemyTurnStatus());

    game.onNewGame(NewGameMsg{Turn::Enemy, Goal::Bottom});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, onKeyboardMouseInputBreakWhenKeyInput)
{
    InSequence s;

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(KeyInput{.key = 'j'})));
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Illegal));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.setCurrentTurn(Turn::User);
    game.setMatchStatus(MatchStatus::InProgress);
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

    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(MouseInput{.x = 2, .y = 1})));
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::nullopt));

    game.onKeyboardMouseInput();
}

TEST_F(GameTest, userKeyWhenKeyNotKnown)
{
    game.userKey('0');
}

TEST_F(GameTest, userKeyWhenGameIsConnecting)
{
    game.setMatchStatus(MatchStatus::Connecting);
    game.userKey('j');
}

TEST_F(GameTest, userKeyWhenGameEnd)
{
    game.setMatchStatus(MatchStatus::GameEnd);
    game.userKey('j');
}

TEST_F(GameTest, userKeyWhenGameInProgress)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Illegal));
    EXPECT_CALL(viewMock, drawBoard());

    game.setMatchStatus(MatchStatus::InProgress);
    game.userKey('j');
}

TEST_F(GameTest, userKeyWhenRequestForNewGameDuringGame)
{
    game.setMatchStatus(MatchStatus::InProgress);
    game.userKey('n');

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
}

TEST_F(GameTest, userKeyWhenRequestForNewGameAfterGame)
{
    EXPECT_CALL(networkMock, sendReadyForNewGame());

    game.setMatchStatus(MatchStatus::GameEnd);
    game.userKey('n');

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::ReadyForNew);
}

TEST_F(GameTest, userMoveWhenEnemyTurn)
{
    game.setCurrentTurn(Turn::Enemy);
    game.userMove(Direction::Left);
}

TEST_F(GameTest, userMoveWhenCurrentUserStatusIsNotContinue)
{
    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Stop);
    game.userMove(Direction::Left);
}

TEST_F(GameTest, userMoveWhenUserTurnAndNextIllegalMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Illegal));
    EXPECT_CALL(viewMock, drawBoard());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userMove(Direction::Left);
}

TEST_F(GameTest, userMoveWhenUserTurnAndNextContinueMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Continue));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userMove(Direction::Left);
}

TEST_F(GameTest, userMoveWhenUserTurnAndNextStopMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Stop));
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setReadyToEndTurnStatus());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userMove(Direction::Left);
}

TEST_F(GameTest, userEndTurnWhenEnemyTurn)
{
    game.setCurrentTurn(Turn::Enemy);
    game.userEndTurn();

    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndContinueMove)
{
    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userEndTurn();

    EXPECT_EQ(game.getCurrentTurn(), Turn::User);
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndDeadEndMove)
{
    EXPECT_CALL(viewMock, setLostStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::DeadEnd);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndTopOwnGoal)
{
    EXPECT_CALL(viewMock, setLostStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Top);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndBottomOwnGoal)
{
    EXPECT_CALL(viewMock, setLostStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Bottom);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndTopGoal)
{
    EXPECT_CALL(viewMock, setWinStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Bottom);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndBottomGoal)
{
    EXPECT_CALL(viewMock, setWinStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Top);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndStopMove)
{
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Stop);
    game.userEndTurn();

    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
}

TEST_F(GameTest, userRequestNewGameWhenGameInProgress)
{
    game.setMatchStatus(MatchStatus::InProgress);
    game.userRequestNewGame();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
}

TEST_F(GameTest, userRequestNewGameWhenGameEnd)
{
    EXPECT_CALL(networkMock, sendReadyForNewGame());

    game.setMatchStatus(MatchStatus::GameEnd);
    game.userRequestNewGame();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::ReadyForNew);
}

TEST_F(GameTest, userRequesNewGmeWhenEnemyReadyForNewGame)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(networkMock, sendNewGame(Turn::User, Goal::Bottom));

    game.setFirstTurn(Turn::User);
    game.setMatchStatus(MatchStatus::EnemyReadyForNew);
    game.userRequestNewGame();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
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

TEST_F(GameTest, onEnemyEndTurnWhenUserTurn)
{
    game.setCurrentTurn(Turn::User);
    ASSERT_ANY_THROW(game.onEnemyEndTurn(EndTurnMsg{}));
}

TEST_F(GameTest, onEnemyEndTurnWhenUEnemyTurnAndEnemyShouldContinue)
{
    game.setCurrentTurn(Turn::Enemy);
    game.setEnemyStatus(MoveStatus::Continue);
    ASSERT_ANY_THROW(game.onEnemyEndTurn(EndTurnMsg{}));
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndTopEnemyGoal)
{
    EXPECT_CALL(viewMock, setLostStatus());

    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Top);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndBottomEnemyGoal)
{
    EXPECT_CALL(viewMock, setLostStatus());

    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Bottom);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndDeadEndMove)
{
    EXPECT_CALL(viewMock, setWinStatus());

    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::DeadEnd);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndTopEnemyOwnGoal)
{
    EXPECT_CALL(viewMock, setWinStatus());

    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Bottom);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndBottomEnemyOwnGoal)
{
    EXPECT_CALL(viewMock, setWinStatus());

    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Top);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnStopMove)
{
    EXPECT_CALL(viewMock, setContinueStatus());

    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::Stop);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getCurrentTurn(), Turn::User);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, onEnemyReadyForNewGameWhenGameInProgress)
{
    game.setMatchStatus(MatchStatus::InProgress);
    game.onEnemyReadyForNewGame(ReadyForNewGameMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
}

TEST_F(GameTest, onEnemyReadyForNewGameWhenGameEnd)
{
    game.setMatchStatus(MatchStatus::GameEnd);
    game.onEnemyReadyForNewGame(ReadyForNewGameMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::EnemyReadyForNew);
}

TEST_F(GameTest, onEnemyReadyForNewGameWhenUserReadyForNewGame)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawBoard());
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(networkMock, sendNewGame(Turn::User, Goal::Bottom));

    game.setFirstTurn(Turn::User);
    game.setMatchStatus(MatchStatus::ReadyForNew);
    game.onEnemyReadyForNewGame(ReadyForNewGameMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

} // namespace PaperSoccer
