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

    void registerHandlers(std::function<void(int)> handleTimerTick) override;
    void start() override;
    void resume() override;
    void stop() override;

    void onTimer(boost::system::error_code errorCode);

private:
    const int DEFAULT_TIME{150};

    boost::asio::high_resolution_timer m_timer;
    std::function<void(int)> m_handleTimerTick;
    int m_timeLeft{DEFAULT_TIME};

};

} // namespace PaperSoccer


#endif // TIMER_HPP
