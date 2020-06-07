#ifndef I_NETWORK_HPP
#define I_NETWORK_HPP

#include <boost/asio.hpp>
#include "TmpMoveMsg.hpp"
#include <functional>


namespace PaperSoccer {

class INetwork {
public:
    virtual ~INetwork() = default;

    virtual void run(std::function<void()> handleKey, std::function<void(const TmpMoveMsg&)>) = 0;
    virtual void send(const TmpMoveMsg& msg) = 0;
};

}
#endif // I_NETWORK_HPP
