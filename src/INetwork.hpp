#ifndef I_NETWORK_HPP
#define I_NETWORK_HPP

#include "Direction.hpp"
#include <functional>

namespace PaperSoccer {

class INetwork {
public:
    virtual ~INetwork() = default;

    virtual void registerHandlers(std::function<void()> handleKeyboardMouseInput,
        std::function<void(const Direction&)> handleEnemyMove) = 0;
    virtual void run() = 0;
    virtual void sendMove(const Direction& dir) = 0;
};

}
#endif // I_NETWORK_HPP
