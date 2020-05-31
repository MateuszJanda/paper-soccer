#ifndef TMP_MOVE_MSG_HPP
#define TMP_MOVE_MSG_HPP

#include "Direction.hpp"

namespace PaperSoccer {

enum class Header : std::int32_t {
    TmpMove = 0
};

struct TmpMoveMsg
{
    TmpMoveMsg(Direction dir);

    const char* data() const
    {
      return data_;
    }

    char* data()
    {
      return data_;
    }

    std::size_t length() const
    {
      return HEADER_LENGTH + BODY_LENGTH;
    }

    Header header;
    Direction dir;

    static const std::size_t HEADER_LENGTH = sizeof(header);
    static const std::size_t BODY_LENGTH = sizeof(dir);
    char data_[HEADER_LENGTH + BODY_LENGTH];
};

}

#endif // TMP_MOVE_MSG_HPP
