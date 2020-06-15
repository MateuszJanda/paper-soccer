#ifndef I_NETWORK_HPP
#define I_NETWORK_HPP

#include "Messages.hpp"
#include <functional>

namespace PaperSoccer {

class INetwork {
public:
    virtual ~INetwork() = default;

    virtual void registerHandlers(std::function<void()> handleKeyboardMouseInput,
        std::function<void()> handleInitNewGame,
        std::function<void(NewGameMsg)> handleNewGame,
        std::function<void(MoveMsg)> handleEnemyMove,
        std::function<void(EndTurnMsg)> handleEnemyEndTurn,
        std::function<void(ReadyForNewGameMsg)> handleReadyForNewGameMsg)
        = 0;
    virtual void run() = 0;
    virtual void sendMove(const Direction& dir) = 0;
    virtual void sendEndTurn() = 0;
    virtual void sendNewGame(Turn turn, Goal goal) = 0;
};

}
#endif // I_NETWORK_HPP
