// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Timer.hpp"

namespace PaperSoccer {

Timer::Timer(boost::asio::io_context& ioContext)
    : m_ioContext{ioContext}
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


    boost::asio::high_resolution_timer tim{m_ioContext};

    //            program.on_clock();
    tim.expires_after(std::chrono::seconds{1});
    tim.async_wait([this](boost::system::error_code errorCode) { onTimer(errorCode); });
}

} // namespace PaperSoccer

