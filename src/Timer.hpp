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
    void start();
    void onTimer(boost::system::error_code errorCode);

private:
    std::function<void()> m_handleTimer;

//    boost::asio::io_context& m_ioContext;
    boost::asio::high_resolution_timer m_tim;

};

} // namespace PaperSoccer


#endif // TIMER_HPP
