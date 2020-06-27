// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef I_TIMER_HPP
#define I_TIMER_HPP

#include <functional>
#include <chrono>

namespace PaperSoccer {

class ITimer {
public:
    virtual ~ITimer() noexcept = default;

    virtual void registerHandler(std::function<void(std::chrono::milliseconds)> handleTimerTick) = 0;
    virtual std::chrono::milliseconds timeLeft() const = 0;
    virtual void start() = 0;
    virtual void resume() = 0;
    virtual void reset() = 0;
    virtual void pause() = 0;
    virtual void pauseAndSync(std::chrono::milliseconds timeLeft) = 0;
};

} // namespace PaperSoccer

#endif // I_TIMER_HPP
