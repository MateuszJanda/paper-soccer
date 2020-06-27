// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Timer.hpp"

namespace PaperSoccer {

Timer::Timer(boost::asio::io_context& ioContext)
    : m_timer{ioContext}
{

}

void Timer::registerHandler(std::function<void(std::chrono::milliseconds)> handleTimerTick)
{
    m_handleTimerTick = handleTimerTick;
}

std::chrono::milliseconds Timer::timeLeft() const
{
    return m_timeLeft;
}

void Timer::start()
{
    m_duration = DEFAULT_DURATION;
    m_timeLeft = DEFAULT_DURATION;
    m_start = std::chrono::steady_clock::now();
    onTimer(boost::system::error_code{});
}

void Timer::resume()
{
    m_start = std::chrono::steady_clock::now();
    onTimer(boost::system::error_code{});
}

void Timer::reset()
{
    m_duration = DEFAULT_DURATION;
    m_timeLeft = DEFAULT_DURATION;
}

void Timer::pause()
{
    m_timer.cancel();
    m_duration = m_timeLeft;
}

void Timer::pauseAndSync(std::chrono::milliseconds timeLeft)
{
    m_timer.cancel();
    m_duration = timeLeft;
    m_timeLeft = timeLeft;
}

void Timer::onTimer(boost::system::error_code errorCode)
{
    using namespace std::chrono;

    if (errorCode) {
        return;
    }

    const auto tickBeforeCall = steady_clock::now();
    const auto timePassed = duration_cast<seconds>(tickBeforeCall - m_start);
    m_timeLeft = m_duration - timePassed;

    if (m_handleTimerTick) {
        const auto left = m_timeLeft.count() <= 0 ? seconds{0} : m_timeLeft;
        m_handleTimerTick(left);
    }

    if (m_timeLeft.count() <= 0) {
        return;
    }

    const auto tickAfterCall = steady_clock::now();
    const auto duration = seconds{1} - duration_cast<milliseconds>(tickAfterCall - tickBeforeCall);
    m_timer.expires_after(duration);
    m_timer.async_wait([this](boost::system::error_code errorCode) { onTimer(errorCode); });
}

} // namespace PaperSoccer

