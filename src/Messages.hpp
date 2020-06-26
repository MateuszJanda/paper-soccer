// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Direction.hpp"
#include "Goal.hpp"
#include "Turn.hpp"

namespace PaperSoccer {

enum class MsgId : std::uint8_t {
    NewGame = 0,
    Move = 1,
    UndoMove = 2,
    EndTurn = 3,
    ReadyForNewGame = 4,
    Timeout = 5,
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

struct UndoMoveMsg {
    UndoMoveMsg()
    {
    }

    static constexpr MsgId msgId{MsgId::UndoMove};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
    }
};


struct EndTurnMsg {
    EndTurnMsg(int timeLeft = 0)
        : timeLeft{timeLeft}
    {
    }

    static constexpr MsgId msgId{MsgId::EndTurn};
    int timeLeft;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & timeLeft;
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

struct TimeoutMsg {
    TimeoutMsg()
    {
    }

    static constexpr MsgId msgId{MsgId::Timeout};

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
    }
};

} // namespace PaperSoccer

#endif // MESSAGES_HPP
