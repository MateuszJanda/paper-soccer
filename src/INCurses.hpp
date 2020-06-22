// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef I_N_CURSES_HPP
#define I_N_CURSES_HPP

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
    virtual ~INCurses() = default;

    virtual void print(int x, int y, std::string str, int colorId = INCurses::DEFAULT) const = 0;
    virtual std::optional<Input> getInput() const = 0;
    virtual void refreshView() const = 0;
    virtual void clearView() const = 0;

    static constexpr int DEFAULT{0};
    static constexpr int RED{1};
    static constexpr int BLUE{2};
    static constexpr int GRAY{3};
    static constexpr int YELLOW{4};
    static constexpr int GREEM{5};
};

} // namespace PaperSoccer

#endif // I_N_CURSES_HPP
