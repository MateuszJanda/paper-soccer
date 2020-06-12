#ifndef I_NETWORK_HPP
#define I_NETWORK_HPP

#include "Direction.hpp"
#include "Turn.hpp"
#include "Goal.hpp"
#include <functional>

namespace PaperSoccer {

class INetwork {
public:
    virtual ~INetwork() = default;

    virtual void registerHandlers(std::function<void()> handleKeyboardMouseInput,
        std::function<void()> handleInitNewGame,
        std::function<void(const Direction&)> handleEnemyMove) = 0;
    virtual void run() = 0;
    virtual void sendMove(const Direction& dir) = 0;
    virtual void sendEndTurn() = 0;
    virtual void sendNewGame(Turn, Goal) = 0;
};

}
#endif // I_NETWORK_HPP
