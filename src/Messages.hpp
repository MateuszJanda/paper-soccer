#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Direction.hpp"
#include "Turn.hpp"
#include "Goal.hpp"
#include <vector>
#include <cstring>

namespace PaperSoccer {

enum class MsgId : std::uint8_t {
    NewGame = 0,
    Move = 1,
    EndTurn = 2
};

struct NewGameMsg
{
    NewGameMsg(Turn turn = Turn::User, Goal goal = Goal::Top)
        : turn{turn},
          goal{goal}
    {
    }

    const MsgId msgId{MsgId::NewGame};
    Turn turn;
    Goal goal;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & turn;
        ar & goal;
    }
};

struct MoveMsg
{
    MoveMsg(Direction dir = Direction::Top)
        : dir{dir}
    {
    }

    const MsgId msgId{MsgId::Move};
    Direction dir{Direction::Top};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & dir;
    }
};

struct EndTurnMsg
{
    EndTurnMsg()
    {
    }

    const MsgId msgId{MsgId::EndTurn};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
    }
};

} // namespace PaperSoccer

#endif // MESSAGES_HPP
