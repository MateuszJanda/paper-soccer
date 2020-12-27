// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Network.hpp"
#include <iomanip>
#include <sstream>

namespace PaperSoccer {

Network::Network(boost::asio::io_context& ioContext)
    : m_ioContext{ioContext}
    , m_socket{ioContext}
    , m_desc{ioContext, 0}
{
}

void Network::registerHandlers(std::function<void()> handleKeyboardMouseInput,
    std::function<void()> handleInitNewGame,
    std::function<void(NewGameMsg)> handleNewGame,
    std::function<void(MoveMsg)> handleEnemyMove,
    std::function<void(UndoMoveMsg)> handleEnemyUndoMove,
    std::function<void(EndTurnMsg)> handleEnemyEndTurn,
    std::function<void(ReadyForNewGameMsg)> handleReadyForNewGameMsg,
    std::function<void(TimeoutMsg)> handleEnemyTimeoutMsg)
{
    m_handleKeyboardMouseInput = handleKeyboardMouseInput;
    m_handleInitNewGame = handleInitNewGame;
    m_handleNewGame = handleNewGame;
    m_handleEnemyMove = handleEnemyMove;
    m_handleEnemyUndoMove = handleEnemyUndoMove;
    m_handleEnemyEndTurn = handleEnemyEndTurn;
    m_handleReadyForNewGameMsg = handleReadyForNewGameMsg;
    m_handleEnemyTimeoutMsg = handleEnemyTimeoutMsg;
}

void Network::setupHandlers()
{
    onRead();
    onKeyboardMouseInput(boost::system::error_code{});
}

void Network::onKeyboardMouseInput(boost::system::error_code errorCode)
{
    if (errorCode) {
        return;
    }

    if (m_handleKeyboardMouseInput) {
        m_handleKeyboardMouseInput();
    }

    using namespace std::placeholders;
    m_desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read,
        [this](boost::system::error_code errorCode) { onKeyboardMouseInput(errorCode); });
}

void Network::sendTimeout()
{
    Message msg;
    msg.mutable_message()->PackFrom(TimeoutMsg{});
    msg.set_msgid(MsgId::Timeout);
    sendMsg(msg);
}

void Network::sendNewGame(Turn turn, Goal goal)
{
    NewGameMsg newGameMsg;
    newGameMsg.set_turn(turn);
    newGameMsg.set_goal(goal);

    Message msg;
    msg.set_msgid(MsgId::NewGame);
    msg.mutable_message()->PackFrom(newGameMsg);

    sendMsg(msg);
}

void Network::sendMove(Direction dir)
{
    MoveMsg moveMsg;
    moveMsg.set_dir(dir);

    Message msg;
    msg.mutable_message()->PackFrom(moveMsg);
    msg.set_msgid(MsgId::Move);

    sendMsg(msg);
}

void Network::sendUndoMove()
{
    Message msg;
    msg.mutable_message()->PackFrom(UndoMoveMsg{});
    msg.set_msgid(MsgId::UndoMove);

    sendMsg(msg);
}

void Network::sendEndTurn(std::chrono::milliseconds timeLeft)
{
    Message msg;
    msg.mutable_message()->PackFrom(EndTurnMsg{});
    msg.set_msgid(MsgId::EndTurn);

    sendMsg(msg);
}

void Network::sendReadyForNewGame()
{
    Message msg;
    msg.mutable_message()->PackFrom(ReadyForNewGameMsg{});
    msg.set_msgid(MsgId::ReadyForNewGame);

    sendMsg(msg);
}

void Network::sendMsg(const Message& msg)
{
    auto data = encodeData(msg);
    auto dataSize = encodeDataSize(data);

    boost::asio::post(m_ioContext,
        [this, ds = std::move(dataSize), d = std::move(data)]() {
            const bool nothingInProgress = m_messageQueue.empty();
            m_messageQueue.push_back(std::move(ds));
            m_messageQueue.push_back(std::move(d));

            if (nothingInProgress) {
                onWrite();
            }
        });
}

std::string Network::encodeData(const Message& msg)
{
    std::string out;
    msg.SerializeToString(&out);

    return out;
}

std::string Network::encodeDataSize(const std::string& data)
{
    const std::string delimiter{" "};
    std::ostringstream dataSizeStream;
    dataSizeStream << "0x" << std::setw(DATA_SIZE_HEX_LENGTH) << std::setfill('0') << std::hex << data.size() << delimiter;

    if (not dataSizeStream || dataSizeStream.str().size() != DATA_SIZE_LENGTH) {
        throw std::length_error{"Can't encode dataSize."};
    }

    return dataSizeStream.str();
}

void Network::onWrite()
{
    boost::asio::async_write(m_socket,
        boost::asio::buffer(m_messageQueue.front()),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            m_messageQueue.pop_front();
            if (not m_messageQueue.empty()) {
                onWrite();
            }
        });
}

void Network::onRead()
{
    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(m_inboundHeader)),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            const auto dataSize = decodeDataSize(std::string{m_inboundHeader.data(), DATA_SIZE_LENGTH});
            onReadMsg(dataSize);
        });
}

void Network::onReadMsg(std::size_t dataSize)
{
    m_inboundData.resize(dataSize);

    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(m_inboundData)),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            Message msg = decodeData(m_inboundData);

            switch (msg.msgid()) {
            case MsgId::NewGame: {

                NewGameMsg newGameMsg;
                msg.message().UnpackTo(&newGameMsg);
                m_handleNewGame(std::move(newGameMsg));
                break;
            }
            case MsgId::Move: {
                MoveMsg moveMsg;
                msg.message().UnpackTo(&moveMsg);
                m_handleEnemyMove(std::move(moveMsg));
                break;
            }
            case MsgId::UndoMove: {
                UndoMoveMsg undoMoveMsg;
                msg.message().UnpackTo(&undoMoveMsg);
                m_handleEnemyUndoMove(std::move(undoMoveMsg));
                break;
            }
            case MsgId::EndTurn: {
                EndTurnMsg endTurnMsg;
                msg.message().UnpackTo(&endTurnMsg);
                m_handleEnemyEndTurn(std::move(endTurnMsg));
                break;
            }
            case MsgId::ReadyForNewGame: {
                ReadyForNewGameMsg readyForNewGameMsg;
                msg.message().UnpackTo(&readyForNewGameMsg);
                m_handleReadyForNewGameMsg(std::move(readyForNewGameMsg));
                break;
            }
            case MsgId::Timeout: {
                TimeoutMsg timeoutMsg;
                msg.message().UnpackTo(&timeoutMsg);
                m_handleEnemyTimeoutMsg(std::move(timeoutMsg));
                break;
            }
            default:
                throw std::invalid_argument{"Can't recognize msgId."};
                break;
            }
            onRead();
        });
}

std::size_t Network::decodeDataSize(const std::string& inboundData)
{
    std::istringstream is{inboundData};
    std::size_t dataSize{0};
    if (!(is >> std::hex >> dataSize)) {
        throw std::invalid_argument{"Can't decode dataSize."};
    }

    return dataSize;
}

Message Network::decodeData(const std::vector<char>& inboundData)
{
    std::string archiveData{&inboundData[0], inboundData.size()};
    Message msg;
    msg.ParseFromString(archiveData);

    return msg;
}

} // namespace PaperSoccer
