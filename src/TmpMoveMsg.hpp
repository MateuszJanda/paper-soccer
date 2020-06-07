#ifndef TMP_MOVE_MSG_HPP
#define TMP_MOVE_MSG_HPP

#include "Direction.hpp"
#include <cstring>

namespace PaperSoccer {

enum class Header : std::uint8_t {
    TmpMove = 0
};

struct TmpMoveMsg {
    TmpMoveMsg(Direction dir);

    uint8_t* data()
    {
        std::memcpy(data_, &header, HEADER_LENGTH);
        std::memcpy(data_ + HEADER_LENGTH, &dir, DIRECTION_LENGTH);
        return data_;
    }

    std::size_t length() const
    {
        return HEADER_LENGTH + DIRECTION_LENGTH;
    }

    bool decode()
    {
        //      std::strncat((char*)&header, (const char*)data_, HEADER_LENGTH);
        header = (Header)data_[0];
        //      std::strncat((char*)&dir, (const char*)data_ + HEADER_LENGTH, DIRECTION_LENGTH);
        dir = (Direction)data_[1];
        return true;
    }

    Header header;
    Direction dir;

    static const std::size_t HEADER_LENGTH = sizeof(header);
    static const std::size_t DIRECTION_LENGTH = sizeof(dir);
    uint8_t data_[HEADER_LENGTH + DIRECTION_LENGTH] = {};
};

}

#endif // TMP_MOVE_MSG_HPP
