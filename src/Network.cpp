// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Network.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
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
    TimeoutMsg msg;
    sendMsg(msg);
}

void Network::sendNewGame(Turn turn, Goal goal)
{
    NewGameMsg msg{turn, goal};
    sendMsg(msg);
}

void Network::sendMove(Direction dir)
{
    MoveMsg msg{dir};
    sendMsg(msg);
}

void Network::sendUndoMove()
{
    UndoMoveMsg msg{};
    sendMsg(msg);
}

void Network::sendEndTurn(std::chrono::milliseconds timeLeft)
{
    EndTurnMsg msg{timeLeft};
    sendMsg(msg);
}

void Network::sendReadyForNewGame()
{
    ReadyForNewGameMsg msg{};
    sendMsg(msg);
}

template <typename Msg>
void Network::sendMsg(const Msg& msg)
{
    auto msgId = encodeMsgId(msg.msgId);
    auto data = encodeData(msg);
    auto dataSize = encodeDataSize(data);

    boost::asio::post(m_ioContext,
        [this, id = std::move(msgId), d = std::move(data), s = std::move(dataSize)]() {
            const bool nothingInProgress = m_messageQueue.empty();
            m_messageQueue.push_back(std::move(id));
            m_messageQueue.push_back(std::move(s));
            m_messageQueue.push_back(std::move(d));

            if (nothingInProgress) {
                onWrite();
            }
        });
}

std::string Network::encodeMsgId(MsgId msgId)
{
    std::ostringstream msgIdStream;
    msgIdStream << std::setw(MSG_ID_LENGTH) << std::hex << static_cast<std::underlying_type_t<MsgId>>(msgId);

    if (not msgIdStream || msgIdStream.str().size() != MSG_ID_LENGTH) {
        throw std::length_error{"Can't encode msgId."};
    }

    return msgIdStream.str();
}

template <typename Msg>
std::string Network::encodeData(const Msg& msg)
{
    std::ostringstream archiveStream;
    boost::archive::text_oarchive archive(archiveStream);
    archive << msg;

    return archiveStream.str();
}

std::string Network::encodeDataSize(const std::string& data)
{
    std::ostringstream dataSizeStream;
    dataSizeStream << std::setw(DATA_SIZE_LENGTH) << std::hex << data.size();

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

            const auto msgId = decodeMsgId(std::string{m_inboundHeader.data(), MSG_ID_LENGTH});
            const auto dataSize = decodeDataSize(std::string{m_inboundHeader.data() + DATA_SIZE_LENGTH, DATA_SIZE_LENGTH});

            switch (msgId) {
            case MsgId::NewGame:
                onReadMsg<NewGameMsg>(dataSize, m_handleNewGame);
                break;
            case MsgId::Move:
                onReadMsg<MoveMsg>(dataSize, m_handleEnemyMove);
                break;
            case MsgId::UndoMove:
                onReadMsg<UndoMoveMsg>(dataSize, m_handleEnemyUndoMove);
                break;
            case MsgId::EndTurn:
                onReadMsg<EndTurnMsg>(dataSize, m_handleEnemyEndTurn);
                break;
            case MsgId::ReadyForNewGame:
                onReadMsg<ReadyForNewGameMsg>(dataSize, m_handleReadyForNewGameMsg);
                break;
            case MsgId::Timeout:
                onReadMsg<TimeoutMsg>(dataSize, m_handleEnemyTimeoutMsg);
                break;
            default:
                throw std::invalid_argument{"Can't recognize msgId."};
                break;
            }
        });
}

template <typename Msg>
void Network::onReadMsg(std::size_t dataSize, std::function<void(Msg)> handlerFunc)
{
    m_inboundData.resize(dataSize);

    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(m_inboundData)),
        [this, handlerFunc](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            Msg msg = decodeData<Msg>(m_inboundData);
            if (handlerFunc) {
                handlerFunc(std::move(msg));
            }

            onRead();
        });
}

MsgId Network::decodeMsgId(const std::string& inboundData)
{
    std::istringstream is{inboundData};
    std::uint8_t msgId{0};
    if (not(is >> std::hex >> msgId)) {
        throw std::invalid_argument{"Can't decode msgId."};
    }

    return static_cast<MsgId>(msgId);
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

template <typename Msg>
Msg Network::decodeData(const std::vector<char>& inboundData)
{
    std::string archiveData{&inboundData[0], inboundData.size()};
    std::istringstream archiveStream{archiveData};
    boost::archive::text_iarchive archive{archiveStream};
    Msg msg;
    archive >> msg;

    return msg;
}

} // namespace PaperSoccer
