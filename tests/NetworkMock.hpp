// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef NETWORK_MOCK_HPP
#define NETWORK_MOCK_HPP

#include "INetwork.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class NetworkMock : public INetwork {
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD7(registerHandlers, void(std::function<void()>,
                                        std::function<void()>,
                                        std::function<void(NewGameMsg)>,
                                        std::function<void(MoveMsg)>,
                                        std::function<void(UndoMoveMsg)>,
                                        std::function<void(EndTurnMsg)>,
                                        std::function<void(ReadyForNewGameMsg)>));
    MOCK_METHOD2(sendNewGame, void(Turn, Goal));
    MOCK_METHOD1(sendMove, void(const Direction&));
    MOCK_METHOD0(sendUndoMove, void());
    MOCK_METHOD0(sendEndTurn, void());
    MOCK_METHOD0(sendReadyForNewGame, void());
};

} // namespace PaperSoccer

#endif // NETWORK_MOCK_HPP
