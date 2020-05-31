#ifndef TMP_MOVE_MSG_HPP
#define TMP_MOVE_MSG_HPP

#include <cstring>
#include "Direction.hpp"

namespace PaperSoccer {

enum class Header : std::int32_t {
    TmpMove = 0
};

struct TmpMoveMsg
{
    TmpMoveMsg(Direction dir);

    char* data()
    {
//      std::memcpy(data_, (void*)header, HEADER_LENGTH);
//      std::memcpy(data_ + HEADER_LENGTH, (void*)dir, DIRECTION_LENGTH);
      return data_;
    }

    std::size_t length() const
    {
      return HEADER_LENGTH + DIRECTION_LENGTH;
    }

    bool decode()
    {
//      std::strncat((char*)header, data_, HEADER_LENGTH);
//      std::strncat((char*)dir, data_ + HEADER_LENGTH, DIRECTION_LENGTH);
      return true;
    }

    Header header;
    Direction dir;

    static const std::size_t HEADER_LENGTH = sizeof(header);
    static const std::size_t DIRECTION_LENGTH = sizeof(dir);
    char data_[HEADER_LENGTH + DIRECTION_LENGTH];
};

}

#endif // TMP_MOVE_MSG_HPP
