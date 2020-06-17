#include "Network.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <type_traits>

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
    std::function<void(ReadyForNewGameMsg)> handleReadyForNewGameMsg)
{
    m_handleKeyboardMouseInput = handleKeyboardMouseInput;
    m_handleInitNewGame = handleInitNewGame;
    m_handleNewGame = handleNewGame;
    m_handleEnemyMove = handleEnemyMove;
    m_handleEnemyUndoMove = handleEnemyUndoMove;
    m_handleEnemyEndTurn = handleEnemyEndTurn;
    m_handleReadyForNewGameMsg = handleReadyForNewGameMsg;
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
        std::bind(&Network::onKeyboardMouseInput, this, _1));
}

void Network::sendNewGame(Turn turn, Goal goal)
{
    NewGameMsg msg{turn, goal};
    sendMsg(msg);
}

void Network::sendMove(const Direction& dir)
{
    MoveMsg msg{dir};
    sendMsg(msg);
}

void Network::sendUndoMove()
{
    UndoMoveMsg msg{};
    sendMsg(msg);
}

void Network::sendEndTurn()
{
    EndTurnMsg msg{};
    sendMsg(msg);
}

void Network::sendReadyForNewGame()
{
    ReadyForNewGameMsg msg{};
    sendMsg(msg);
}

template<typename Msg>
void Network::sendMsg(const Msg& msg)
{
    outbound_msgId = encodeMsgId(msg.msgId);
    outbound_data_ = encodeData(msg);
    outbound_header_ = encodeDataSize(outbound_data_);

    boost::asio::post(m_ioContext,
        [this]() {
            bool writeInProgress = not m_messageQueue.empty();
            m_messageQueue.push_back(boost::asio::buffer(outbound_msgId));
            m_messageQueue.push_back(boost::asio::buffer(outbound_header_));
            m_messageQueue.push_back(boost::asio::buffer(outbound_data_));
            if (not writeInProgress) {
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

template<typename Msg>
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
        m_messageQueue.front(),
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
        boost::asio::buffer(boost::asio::buffer(inbound_hhh)),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            const auto msgId = decodeMsgId(std::string{inbound_hhh, MSG_ID_LENGTH});
            const auto dataSize = decodeDataSize(std::string{inbound_hhh + DATA_SIZE_LENGTH, DATA_SIZE_LENGTH});

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
            default:
                throw std::invalid_argument{"Can't recognize msgId."};
                break;
            }
        });
}

MsgId Network::decodeMsgId(const std::string& inboundData)
{
    std::istringstream is{inboundData};
    std::uint8_t msgId{0};
    if (not (is >> std::hex >> msgId)) {
        throw std::invalid_argument{"Can't decode msgId."};
    }

    return  static_cast<MsgId>(msgId);
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

template<typename Msg>
void Network::onReadMsg(std::size_t dataSize, std::function<void(Msg)> handlerFunc)
{
    inbound_data_.resize(dataSize);

    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(inbound_data_)),
        [this, handlerFunc](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            Msg msg = decodeData<Msg>(inbound_data_);
            if (handlerFunc) {
                handlerFunc(msg);
            }

            onRead();
        });
}

template<typename Msg>
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
