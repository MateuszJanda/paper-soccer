// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef I_N_CURSES_HPP
#define I_N_CURSES_HPP

#include "ColorPair.hpp"
#include <optional>
#include <string>
#include <variant>

namespace PaperSoccer {

struct KeyInput {
    int key;
};

struct EnterInput {
};

struct MouseInput {
    int x;
    int y;
};

using Input = std::variant<KeyInput, EnterInput, MouseInput>;

class INCurses {
public:
    virtual ~INCurses() noexcept = default;

    virtual void print(int x, int y, std::string str, ColorPair colorPair = ColorPair::DEFAULT) const = 0;
    virtual std::optional<Input> getInput() const noexcept = 0;
    virtual void refreshView() const noexcept = 0;
    virtual void clearView() const noexcept = 0;
};

} // namespace PaperSoccer

#endif // I_N_CURSES_HPP
