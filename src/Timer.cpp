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

void Timer::registerHandler(std::function<void(std::chrono::seconds)> handleTimerTick)
{
    m_handleTimerTick = handleTimerTick;
}

std::chrono::seconds Timer::timeLeft() const
{
    return m_timeLeft;
}

void Timer::start()
{
    m_timeLeft = DEFAULT_TIME;
    onTimer(boost::system::error_code{});
}

void Timer::resume()
{
    onTimer(boost::system::error_code{});
}

void Timer::stop()
{
    m_timer.cancel();
}

void Timer::reset()
{
    m_timeLeft = DEFAULT_TIME;
}

void Timer::stopAndSync(std::chrono::seconds timeLeft)
{
    m_timeLeft = timeLeft;
    m_timer.cancel();
}

void Timer::onTimer(boost::system::error_code errorCode)
{
    if (errorCode) {
        return;
    }

    m_timeLeft--;

    if (m_handleTimerTick) {
        m_handleTimerTick(m_timeLeft);
    }

    if (m_timeLeft.count() <= 0) {
        return;
    }

    m_timer.expires_after(std::chrono::seconds{1});
    m_timer.async_wait([this](boost::system::error_code errorCode) { onTimer(errorCode); });
}

} // namespace PaperSoccer

