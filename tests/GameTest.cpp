#include "Game.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "NetworkMock.hpp"
#include "ViewMock.hpp"
#include "gtest/gtest.h"

namespace PaperSoccer {

namespace {
    const std::vector<Direction> EMPTY_PATH{};
    const std::vector<Direction> DIR_PATH{Direction::Left};
} // namespace anonymous

using namespace testing;

class TestableGame : public Game {
public:
    TestableGame(INetwork& network, IBoard& board, INCurses& ncurses, IView& view)
        : Game(network, board, ncurses, view)
    {
    }

    void setCurrentTurn(Turn turn)
    {
        m_currentTurn = turn;
    }

    Turn getCurrentTurn() const
    {
        return m_currentTurn;
    }

    void setFirstTurn(Turn turn)
    {
        m_firstTurn = turn;
    }

    void setUserStatus(MoveStatus status)
    {
        m_userStatus = status;
    }

    MoveStatus getUserStatus() const
    {
        return m_userStatus;
    }

    void setEnemyStatus(MoveStatus status)
    {
        m_enemyStatus = status;
    }

    MoveStatus getEnemyStatus() const
    {
        return m_enemyStatus;
    }

    void setUserGoal(Goal goal)
    {
        m_userGoal = goal;
    }

    void setMatchStatus(MatchStatus status)
    {
        m_match = status;
    }

    MatchStatus getMatchStatus() const
    {
        return m_match;
    }

    void setDirectionPath(std::vector<Direction> dirPath)
    {
        m_dirPath = dirPath;
    }

    std::vector<Direction> getDirectionPath() const
    {
        return m_dirPath;
    }
};

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
    TestableGame game;
};

TEST_F(GameTest, run)
{
    using namespace std::placeholders;

    EXPECT_CALL(networkMock, registerHandlers(_, _, _, _, _, _, _));
    EXPECT_CALL(networkMock, run());

    game.run();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::Connecting);
}

TEST_F(GameTest, initNewGameWhenFirstTurnIsUser)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawLegend(_, _, _));
    EXPECT_CALL(viewMock, drawScore(_, _));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
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
    EXPECT_CALL(viewMock, drawLegend(_, _, _));
    EXPECT_CALL(viewMock, drawScore(_, _));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
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
    EXPECT_CALL(viewMock, drawLegend(_, _, _));
    EXPECT_CALL(viewMock, drawScore(_, _));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(viewMock, setContinueStatus());

    game.onNewGame(NewGameMsg{Turn::User, Goal::Bottom});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::User);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, onNewGameWhenEnemyTurn)
{
    EXPECT_CALL(boardMock, reset());
    EXPECT_CALL(viewMock, drawLegend(_, _, _));
    EXPECT_CALL(viewMock, drawScore(_, _));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
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

    const int x = 2;
    const int y = 1;
    EXPECT_CALL(ncursesMock, getInput()).WillOnce(Return(std::make_optional(MouseInput{.x = x, .y = y})));
    EXPECT_CALL(viewMock, isStatusButton(x, y)).WillOnce(Return(false));
    EXPECT_CALL(viewMock, getMoveDirection(x, y)).WillOnce(Return(std::nullopt));
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

    game.setMatchStatus(MatchStatus::InProgress);
    game.setUserStatus(MoveStatus::Continue);
    game.userKey('j');

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, userKeyWhenRequestForNewGameDuringGame)
{
    game.setMatchStatus(MatchStatus::InProgress);
    game.userKey(Game::NEW_GAME_KEY);

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
}

TEST_F(GameTest, userKeyWhenRequestForNewGameAfterGame)
{
    EXPECT_CALL(networkMock, sendReadyForNewGame());

    game.setMatchStatus(MatchStatus::GameEnd);
    game.userKey(Game::NEW_GAME_KEY);

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::ReadyForNew);
}

TEST_F(GameTest, userKeyWhenUndoMoveInEnemyTurn)
{
    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.userKey(Game::UNDO_MOVE_KEY);

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, userKeyWhenUndoMoveInUserTurn)
{
    EXPECT_CALL(boardMock, undoBallMove(Direction::Bottom));
    EXPECT_CALL(networkMock, sendUndoMove());
    EXPECT_CALL(viewMock, setContinueStatus());
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Stop);
    game.setDirectionPath({Direction::Top});
    game.userKey(Game::UNDO_MOVE_KEY);

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
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

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userMove(Direction::Left);

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, userMoveWhenUserTurnAndNextContinueMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Continue));
    EXPECT_CALL(viewMock, drawBoard(DIR_PATH));
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userMove(Direction::Left);

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, userMoveWhenUserTurnAndNextStopMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Stop));
    EXPECT_CALL(viewMock, drawBoard(DIR_PATH));
    EXPECT_CALL(viewMock, setReadyToEndTurnStatus());
    EXPECT_CALL(networkMock, sendMove(Direction::Left));

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userMove(Direction::Left);

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, userMouseWhenStatusButtonClickAndEndTurn)
{
    const int x = 1;
    const int y = 2;
    EXPECT_CALL(viewMock, isStatusButton(x, y)).WillOnce(Return(true));
    EXPECT_CALL(viewMock, setLostStatus(0, 1));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::DeadEnd);
    game.userMouse(x, y);

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, userMouseWhenCorrectMoveClick)
{
    const int x = 1;
    const int y = 2;
    EXPECT_CALL(viewMock, isStatusButton(x, y)).WillOnce(Return(false));
    EXPECT_CALL(viewMock, getMoveDirection(x, y)).WillOnce(Return(Direction::Left));
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Illegal));

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Continue);
    game.userMouse(x, y);

    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, userEndTurnWhenEnemyTurn)
{
    game.setCurrentTurn(Turn::Enemy);
    game.setDirectionPath({Direction::Top});
    game.userEndTurn();

    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre(Direction::Top));
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndContinueMove)
{
    game.setCurrentTurn(Turn::User);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Continue);
    game.userEndTurn();

    EXPECT_EQ(game.getCurrentTurn(), Turn::User);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre(Direction::Top));
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndDeadEndMove)
{
    EXPECT_CALL(viewMock, setLostStatus(0, 1));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::DeadEnd);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndTopOwnGoal)
{
    EXPECT_CALL(viewMock, setLostStatus(0, 1));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Top);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndBottomOwnGoal)
{
    EXPECT_CALL(viewMock, setLostStatus(0, 1));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Bottom);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndTopGoal)
{
    EXPECT_CALL(viewMock, setWinStatus(1, 0));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Bottom);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndBottomGoal)
{
    EXPECT_CALL(viewMock, setWinStatus(1, 0));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Top);
    game.userEndTurn();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, userEndTurnWhenUserTurnAndStopMove)
{
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendEndTurn());

    game.setCurrentTurn(Turn::User);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Stop);
    game.userEndTurn();

    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
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
    EXPECT_CALL(viewMock, drawLegend(_, _, _));
    EXPECT_CALL(viewMock, drawScore(0, 0));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(networkMock, sendNewGame(Turn::User, Goal::Bottom));

    game.setFirstTurn(Turn::User);
    game.setMatchStatus(MatchStatus::EnemyReadyForNew);
    game.userRequestNewGame();

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::InProgress);
    EXPECT_EQ(game.getCurrentTurn(), Turn::Enemy);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
}

TEST_F(GameTest, userUndoMoveWhenEnemyTurn)
{
    game.setCurrentTurn(Turn::Enemy);
    game.setUserStatus(MoveStatus::Stop);
    game.setDirectionPath({Direction::Top});
    game.userUndoMove();

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre(Direction::Top));
}

TEST_F(GameTest, userUndoMoveWhenUserTurnWithEmptyDirPath)
{
    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Stop);
    game.setDirectionPath(EMPTY_PATH);
    game.userUndoMove();

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, userUndoMoveWhenUserTurnWithDirPath)
{
    EXPECT_CALL(boardMock, undoBallMove(Direction::Bottom));
    EXPECT_CALL(viewMock, setContinueStatus());
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
    EXPECT_CALL(networkMock, sendUndoMove());

    game.setCurrentTurn(Turn::User);
    game.setUserStatus(MoveStatus::Stop);
    game.setDirectionPath({Direction::Top});
    game.userUndoMove();

    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, onEnemyMoveWhenUserTurn)
{
    game.setCurrentTurn(Turn::User);

    ASSERT_ANY_THROW(game.onEnemyMove(Direction::Top));
}

TEST_F(GameTest, onEnemyMoveWhenEnemyTurnAndIllegalMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Top)).WillOnce(Return(MoveStatus::Illegal));
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());

    game.setCurrentTurn(Turn::Enemy);
    ASSERT_ANY_THROW(game.onEnemyMove(Direction::Top));
}

TEST_F(GameTest, onEnemyMoveWhenEnemyTurnAndLegalMove)
{
    EXPECT_CALL(boardMock, moveBall(Direction::Left)).WillOnce(Return(MoveStatus::Continue));
    EXPECT_CALL(viewMock, drawBoard(DIR_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.onEnemyMove(Direction::Left);

    EXPECT_THAT(game.getDirectionPath(), ElementsAre(Direction::Left));
}

TEST_F(GameTest, onEnemyUndoMoveWhenUserTurn)
{
    game.setCurrentTurn(Turn::User);
    game.setEnemyStatus(MoveStatus::Stop);
    game.setDirectionPath({Direction::Top});
    game.onEnemyUndoMove(UndoMoveMsg{});

    EXPECT_EQ(game.getEnemyStatus(), MoveStatus::Stop);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre(Direction::Top));
}

TEST_F(GameTest, onEnemyUndoMoveWhenEnemyTurnWithEmptyDirPath)
{
    game.setCurrentTurn(Turn::Enemy);
    game.setEnemyStatus(MoveStatus::Stop);
    game.setDirectionPath(EMPTY_PATH);
    game.onEnemyUndoMove(UndoMoveMsg{});

    EXPECT_EQ(game.getEnemyStatus(), MoveStatus::Stop);
}

TEST_F(GameTest, onEnemyUndoMoveWhenEnemyTurnWithDirPath)
{
    EXPECT_CALL(boardMock, undoBallMove(Direction::Right));
    EXPECT_CALL(viewMock, setEnemyTurnStatus());
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.setEnemyStatus(MoveStatus::Stop);
    game.setDirectionPath(DIR_PATH);
    game.onEnemyUndoMove(UndoMoveMsg{});

    EXPECT_EQ(game.getEnemyStatus(), MoveStatus::Continue);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
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
    EXPECT_CALL(viewMock, setLostStatus(0, 1));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Top);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndBottomEnemyGoal)
{
    EXPECT_CALL(viewMock, setLostStatus(0, 1));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Bottom);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndDeadEndMove)
{
    EXPECT_CALL(viewMock, setWinStatus(1, 0));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::DeadEnd);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndTopEnemyOwnGoal)
{
    EXPECT_CALL(viewMock, setWinStatus(1, 0));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::TopGoal);
    game.setUserGoal(Goal::Bottom);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnAndBottomEnemyOwnGoal)
{
    EXPECT_CALL(viewMock, setWinStatus(1, 0));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::BottomGoal);
    game.setUserGoal(Goal::Top);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getMatchStatus(), MatchStatus::GameEnd);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Stop);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
}

TEST_F(GameTest, onEnemyEndTurnWhenEnemyTurnStopMove)
{
    EXPECT_CALL(viewMock, setContinueStatus());
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));

    game.setCurrentTurn(Turn::Enemy);
    game.setDirectionPath({Direction::Top});
    game.setUserStatus(MoveStatus::Stop);
    game.setEnemyStatus(MoveStatus::Stop);
    game.onEnemyEndTurn(EndTurnMsg{});

    EXPECT_EQ(game.getCurrentTurn(), Turn::User);
    EXPECT_EQ(game.getUserStatus(), MoveStatus::Continue);
    EXPECT_THAT(game.getDirectionPath(), ElementsAre());
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
    EXPECT_CALL(viewMock, drawLegend(_, _, _));
    EXPECT_CALL(viewMock, drawScore(0, 0));
    EXPECT_CALL(viewMock, drawBoard(EMPTY_PATH));
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
