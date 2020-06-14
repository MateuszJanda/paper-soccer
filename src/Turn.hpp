#ifndef TURN_HPP
#define TURN_HPP

#include <cstdint>

namespace PaperSoccer {

enum class Turn : std::uint8_t {
    User,
    Enemy,
};

} // namespace PaperSoccer

#endif // TURN_HPP
