#ifndef NETWORK_MOCK
#define NETWORK_MOCK

#include "INetwork.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class NetworkMock : public INetwork {
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD1(send, void(const TmpMoveMsg&));
    MOCK_METHOD2(registerHandlers, void(std::function<void()>,
                                        std::function<void(const TmpMoveMsg&)>));
};

} // namespace PaperSoccer

#endif // NETWORK_MOCK
