#ifndef I_NETWORK_HPP
#define I_NETWORK_HPP

#include "TmpMoveMsg.hpp"
#include <functional>

namespace PaperSoccer {

class INetwork {
public:
    virtual ~INetwork() = default;

    virtual void registerHandlers(std::function<void()> handleKeyboardMouseInput,
                                  std::function<void(const TmpMoveMsg&)> handleMove) = 0;
    virtual void run() = 0;
    virtual void send(const TmpMoveMsg& msg) = 0;
};

}
#endif // I_NETWORK_HPP
