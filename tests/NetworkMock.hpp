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
    MOCK_METHOD(void, run, ());
    MOCK_METHOD(void, registerHandlers, (std::function<void()>,
                                         std::function<void()>,
                                         std::function<void(NewGameMsg)>,
                                         std::function<void(MoveMsg)>,
                                         std::function<void(UndoMoveMsg)>,
                                         std::function<void(EndTurnMsg)>,
                                         std::function<void(ReadyForNewGameMsg)>,
                                         std::function<void(TimeoutMsg)>));
    MOCK_METHOD(void, sendTimeout, ());
    MOCK_METHOD(void, sendNewGame, (Turn, Goal));
    MOCK_METHOD(void, sendMove, (const Direction&));
    MOCK_METHOD(void, sendUndoMove, ());
    MOCK_METHOD(void, sendEndTurn, (std::chrono::seconds));
    MOCK_METHOD(void, sendReadyForNewGame, ());
};

} // namespace PaperSoccer

#endif // NETWORK_MOCK_HPP
