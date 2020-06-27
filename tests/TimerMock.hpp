// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef TIMER_MOCK_HPP
#define TIMER_MOCK_HPP

#include "ITimer.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class TimerMock : public ITimer {
public:
    MOCK_METHOD(void, registerHandler, (std::function<void(std::chrono::milliseconds)>));
    MOCK_METHOD(std::chrono::milliseconds, timeLeft, (), (const));
    MOCK_METHOD(void, start, ());
    MOCK_METHOD(void, resume, ());
    MOCK_METHOD(void, reset, ());
    MOCK_METHOD(void, pause, ());
    MOCK_METHOD(void, pauseAndSync, (std::chrono::milliseconds));
};

} // namespace PaperSoccer

#endif // TIMER_MOCK_HPP
