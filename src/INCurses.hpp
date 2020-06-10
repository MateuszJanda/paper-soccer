#ifndef I_N_CURSES_HPP
#define I_N_CURSES_HPP

#include <string>
#include <variant>
#include <optional>

namespace PaperSoccer {


struct KeyData {
    int key;
};

struct MouseData
{
    int x;
    int y;
};


using Input = std::variant<KeyData, MouseData>;

class INCurses {
public:
    virtual ~INCurses() = default;

    virtual void print(int x, int y, std::string str) = 0;
    virtual std::optional<Input> getInput() = 0;
    virtual void refreshView() = 0;
};

} // namespace PaperSoccer

#endif // I_N_CURSES_HPP
