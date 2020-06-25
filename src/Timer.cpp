// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Timer.hpp"

namespace PaperSoccer {

Timer::Timer(boost::asio::io_context& ioContext)
    : m_tim{ioContext}
{

}

void Timer::start()
{
}

void Timer::onTimer(boost::system::error_code errorCode)
{
    if (errorCode) {
        return;
    }

    if (m_handleTimer) {
        m_handleTimer();
    }

    m_tim.expires_after(std::chrono::seconds{1});
    m_tim.async_wait([this](boost::system::error_code errorCode) { onTimer(errorCode); });
}

} // namespace PaperSoccer

