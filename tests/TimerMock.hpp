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
};

} // namespace PaperSoccer


#endif // TIMER_MOCK_HPP
