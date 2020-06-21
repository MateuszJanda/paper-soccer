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

    virtual void print(int x, int y, std::string str) = 0;
    virtual std::optional<Input> getInput() = 0;
    virtual void refreshView() = 0;
    virtual void clearView() = 0;
};

} // namespace PaperSoccer

#endif // I_N_CURSES_HPP
