#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Direction.hpp"
#include "Goal.hpp"
#include "Turn.hpp"

namespace PaperSoccer {

enum class MsgId : std::uint8_t {
    NewGame = 0,
    Move = 1,
    EndTurn = 2,
    ReadyForNewGame = 3
};

struct NewGameMsg {
    NewGameMsg(Turn turn = Turn::User, Goal goal = Goal::Top)
        : turn{turn}
        , goal{goal}
    {
    }

    static constexpr MsgId msgId{MsgId::NewGame};
    Turn turn;
    Goal goal;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& turn;
        ar& goal;
    }
};

struct MoveMsg {
    MoveMsg(Direction dir = Direction::Top)
        : dir{dir}
    {
    }

    static constexpr MsgId msgId{MsgId::Move};
    Direction dir{Direction::Top};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& dir;
    }
};

struct EndTurnMsg {
    EndTurnMsg()
    {
    }

    static constexpr MsgId msgId{MsgId::EndTurn};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
    }
};

struct ReadyForNewGameMsg {
    ReadyForNewGameMsg()
    {
    }

    static constexpr MsgId msgId{MsgId::ReadyForNewGame};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
    }
};

} // namespace PaperSoccer

#endif // MESSAGES_HPP
