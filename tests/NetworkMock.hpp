#ifndef NETWORK_MOCK_HPP
#define NETWORK_MOCK_HPP

#include "INetwork.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class NetworkMock : public INetwork {
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD3(registerHandlers, void(std::function<void()>,
        std::function<void()>,
        std::function<void(const Direction&)>));
    MOCK_METHOD1(sendMove, void(const Direction&));
    MOCK_METHOD0(sendEndTurn, void());
    MOCK_METHOD2(sendNewGame, void(Turn, Goal));
};

} // namespace PaperSoccer

#endif // NETWORK_MOCK_HPP
