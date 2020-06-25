// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef TIMER_HPP
#define TIMER_HPP

#include "ITimer.hpp"
#include <boost/asio.hpp>

namespace PaperSoccer {

class Timer : public ITimer {
public:
    Timer(boost::asio::io_context& ioContext);
    void start();
    void onTimer(boost::system::error_code errorCode);

private:
    boost::asio::io_context& m_ioContext;
};

} // namespace PaperSoccer


#endif // TIMER_HPP
