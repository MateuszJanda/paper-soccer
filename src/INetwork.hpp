// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef I_NETWORK_HPP
#define I_NETWORK_HPP

#include "Messages.hpp"
#include <functional>

namespace PaperSoccer {

class INetwork {
public:
    virtual ~INetwork() noexcept = default;

    virtual void registerHandlers(std::function<void()> handleKeyboardMouseInput,
        std::function<void()> handleInitNewGame,
        std::function<void(NewGameMsg)> handleNewGame,
        std::function<void(MoveMsg)> handleEnemyMove,
        std::function<void(UndoMoveMsg)> handleEnemyUndoMove,
        std::function<void(EndTurnMsg)> handleEnemyEndTurn,
        std::function<void(ReadyForNewGameMsg)> handleReadyForNewGameMsg,
        std::function<void(TimeoutMsg)> handleEnemyTimeout)
        = 0;
    virtual void run() = 0;
    virtual void sendNewGame(Turn turn, Goal goal) = 0;
    virtual void sendMove(const Direction& dir) = 0;
    virtual void sendUndoMove() = 0;
    virtual void sendEndTurn(std::chrono::milliseconds timeLeft) = 0;
    virtual void sendReadyForNewGame() = 0;

    virtual void sendTimeout() = 0;
};

} // namespace PaperSoccer

#endif // I_NETWORK_HPP
