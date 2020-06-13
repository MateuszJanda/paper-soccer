#ifndef MOVE_MSG_HPP
#define MOVE_MSG_HPP

#include "Direction.hpp"
#include <vector>
#include <cstring>

namespace PaperSoccer {

enum class MsgId : std::uint8_t {
    MoveMsg = 0
};

struct HeaderMsg1 {
    std::uint16_t dataSize{0};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & dataSize;
    }
};

struct MoveMsg1
{
    MoveMsg1(Direction dir)
        : dir{dir}
    {

    }

    MsgId msgId{MsgId::MoveMsg};
    Direction dir{Direction::Top};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & msgId;
        ar & dir;
    }
};

struct HeaderMsg {
    HeaderMsg(MsgId msgId) : msgId{msgId}
    {
    }

    std::size_t length() const
    {
        return sizeof(msgId) + sizeof(dataSize);
    }

    std::vector<std::uint8_t> encode()
    {
        std::vector<std::uint8_t> buff(length(), 0);
        std::memcpy(buff.data(), &msgId, sizeof(msgId));
        std::memcpy(buff.data() + sizeof(dataSize), &dataSize, sizeof(dataSize));

        return buff;
    }

    void decode(std::vector<std::uint8_t> buff)
    {
        msgId = *reinterpret_cast<MsgId*>(buff.data());
        dataSize = *reinterpret_cast<std::uint16_t*>(buff.data() + sizeof(msgId));
    }

    MsgId msgId{MsgId::MoveMsg};
    std::uint16_t dataSize{0};
};

struct MoveMsg : HeaderMsg {
    MoveMsg(Direction dir)
        : HeaderMsg{MsgId::MoveMsg}
        , dir{dir}
    {
    }

    uint8_t* data()
    {
        std::memcpy(data_, &msgId, sizeof(msgId));
        std::memcpy(data_ + sizeof(msgId), &dir, DIRECTION_LENGTH);
        return data_;
    }

    std::size_t length() const
    {
        return HeaderMsg::length() + DIRECTION_LENGTH;
    }

    std::vector<std::uint8_t> encode()
    {
        std::vector<std::uint8_t> buff(length(), 0);

        return buff;
    }

    bool decode()
    {
        //      std::strncat((char*)&header, (const char*)data_, HEADER_LENGTH);
        msgId = (MsgId)data_[0];
        //      std::strncat((char*)&dir, (const char*)data_ + HEADER_LENGTH, DIRECTION_LENGTH);
        dir = (Direction)data_[1];
        return true;
    }

    Direction dir;

    static const std::size_t DIRECTION_LENGTH = sizeof(dir);
    uint8_t data_[sizeof(msgId) + DIRECTION_LENGTH] = {};
};

} // namespace PaperSoccer

#endif // MOVE_MSG_HPP
