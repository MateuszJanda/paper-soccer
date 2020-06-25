// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam


#ifndef I_TIMER_HPP
#define I_TIMER_HPP

namespace PaperSoccer {

class ITimer {
public:
    virtual ~ITimer() noexcept = default;
};

} // namespace PaperSoccer

#endif // I_TIMER_HPP
