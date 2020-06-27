// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "INetwork.hpp"
#include "Messages.hpp"
#include <boost/asio.hpp>
#include <deque>
#include <functional>

namespace PaperSoccer {

class Network : public INetwork {
public:
    Network(boost::asio::io_context& ioContext);

    void registerHandlers(std::function<void()> handleKeyboardMouseInput,
        std::function<void()> handleInitNewGame,
        std::function<void(NewGameMsg)> handleNewGame,
        std::function<void(MoveMsg)> handleEnemyMove,
        std::function<void(UndoMoveMsg)> handleEnemyUndoMove,
        std::function<void(EndTurnMsg)> handleEnemyEndTurn,
        std::function<void(ReadyForNewGameMsg)> handleReadyForNewGameMsg,
        std::function<void(TimeoutMsg)> handleEnemyTimeout) override;

    void onKeyboardMouseInput(boost::system::error_code errorCode);

    void sendTimeout() override;

    void sendNewGame(Turn turn, Goal goal) override;
    void sendMove(const Direction& dir) override;
    void sendUndoMove() override;
    void sendEndTurn(std::chrono::milliseconds timeLeft) override;
    void sendReadyForNewGame() override;

    template<typename Msg>
    void sendMsg(const Msg& msg);
    void onWrite();

    void onRead();
    template<typename Msg>
    void onReadMsg(std::size_t dataSize, std::function<void(Msg)> handlerFunc);

protected:
    void setupHandlers();

    boost::asio::ip::tcp::socket m_socket;
    std::function<void()> m_handleInitNewGame;

private:
    std::string encodeMsgId(MsgId msgId);
    std::string encodeDataSize(const std::string& data);
    template<typename Msg>
    std::string encodeData(const Msg& msg);

    MsgId decodeMsgId(const std::string& inboundData);
    std::size_t decodeDataSize(const std::string& inboundData);
    template<typename Msg>
    Msg decodeData(const std::vector<char>& inboundData);

    boost::asio::io_context& m_ioContext;
    boost::asio::posix::stream_descriptor m_desc;

    std::function<void()> m_handleKeyboardMouseInput;
    std::function<void(NewGameMsg)> m_handleNewGame;
    std::function<void(MoveMsg)> m_handleEnemyMove;
    std::function<void(UndoMoveMsg)> m_handleEnemyUndoMove;
    std::function<void(EndTurnMsg)> m_handleEnemyEndTurn;
    std::function<void(ReadyForNewGameMsg)> m_handleReadyForNewGameMsg;
    std::function<void(TimeoutMsg)> m_handleEnemyTimeoutMsg;

    static constexpr int MSG_ID_LENGTH{8};
    static constexpr int DATA_SIZE_LENGTH{8};

    std::array<char, MSG_ID_LENGTH + DATA_SIZE_LENGTH> m_inboundHeader;
    std::vector<char> m_inboundData;

    std::deque<std::string> m_messageQueue;
};

} // namespace PaperSoccer

#endif // NETWORK_HPP
