// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef I_TIMER_HPP
#define I_TIMER_HPP

#include <functional>

namespace PaperSoccer {

class ITimer {
public:
    virtual ~ITimer() noexcept = default;

    virtual void registerHandlers(std::function<void(int)> handleTimerTick) = 0;
    virtual void start() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
};

} // namespace PaperSoccer

#endif // I_TIMER_HPP
