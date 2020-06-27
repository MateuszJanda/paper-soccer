// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef TIMER_HPP
#define TIMER_HPP

#include "ITimer.hpp"
#include <boost/asio.hpp>
#include <functional>

namespace PaperSoccer {

class Timer : public ITimer {
public:
    Timer(boost::asio::io_context& ioContext);

    void registerHandler(std::function<void(std::chrono::milliseconds)> handleTimerTick) override;
    std::chrono::milliseconds timeLeft() const override;
    void start() override;
    void resume() override;
    void reset() override;
    void pause() override;
    void pauseAndSync(std::chrono::milliseconds timeLeft) override;

    void onTimer(boost::system::error_code errorCode);

private:
    const std::chrono::seconds DEFAULT_DURATION{150};

    boost::asio::high_resolution_timer m_timer;
    std::function<void(std::chrono::milliseconds)> m_handleTimerTick;
    std::chrono::milliseconds m_timeLeft{DEFAULT_DURATION};
    std::chrono::milliseconds m_duration{DEFAULT_DURATION};
    std::chrono::steady_clock::time_point m_start;
};

} // namespace PaperSoccer


#endif // TIMER_HPP
