// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "DirectionUtils.hpp"
#include "Goal.pb.h"
#include "Turn.pb.h"
#include "MsgId.pb.h"
#include <chrono>

namespace PaperSoccer {

struct NewGameMsg {
    NewGameMsg(Turn turn = Turn::User, Goal goal = Goal::Up)
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
    EndTurnMsg(std::chrono::milliseconds timeLeft = std::chrono::milliseconds{0})
        : timeLeft{static_cast<int>(timeLeft.count())}
    {
    }

    static constexpr MsgId msgId{MsgId::EndTurn};
    int timeLeft;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& timeLeft;
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
