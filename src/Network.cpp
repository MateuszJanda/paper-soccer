#include "Network.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <memory>
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
    std::function<void(const Turn&, const Goal&)> handleNewGame,
    std::function<void(const Direction&)> handleEnemyMove,
    std::function<void()> handleEnemyEndTurn)
{
    m_handleKeyboardMouseInput = handleKeyboardMouseInput;
    m_handleInitNewGame = handleInitNewGame;
    m_handleNewGame = handleNewGame;
    m_handleEnemyMove = handleEnemyMove;
    m_handleEnemyEndTurn = handleEnemyEndTurn;
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

void Network::sendEndTurn()
{
    EndTurnMsg msg{};
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
                onReadNewGameMsg(dataSize);
                break;
            case MsgId::Move:
                onReadMoveMsg(dataSize);
                break;
            case MsgId::EndTurn:
                onReadEndTurnMsg(dataSize);
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

void Network::onReadNewGameMsg(std::size_t inbound_data_size)
{
    inbound_data_.resize(inbound_data_size);

    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(inbound_data_)),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            NewGameMsg msg;
            try {
                std::string archive_data(&inbound_data_[0], inbound_data_.size());
                std::istringstream archiveStream(archive_data);
                boost::archive::text_iarchive archive(archiveStream);
                archive >> msg;
            } catch (std::exception& e) {
                // Unable to decode data.
                //              boost::system::error_code error(boost::asio::error::invalid_argument);
                //              boost::get<0>(handler)(error);
                return;
            }

            if (m_handleNewGame) {
                m_handleNewGame(msg.turn, msg.goal);
            }

            onRead();
        });
}

void Network::onReadMoveMsg(std::size_t inbound_data_size)
{
    inbound_data_.resize(inbound_data_size);

    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(inbound_data_)),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            MoveMsg msg;
            try {
                std::string archive_data(&inbound_data_[0], inbound_data_.size());
                std::istringstream archiveStream(archive_data);
                boost::archive::text_iarchive archive(archiveStream);
                archive >> msg;
            } catch (std::exception& e) {
                // Unable to decode data.
                //              boost::system::error_code error(boost::asio::error::invalid_argument);
                //              boost::get<0>(handler)(error);
                return;
            }

            if (m_handleEnemyMove) {
                m_handleEnemyMove(msg.dir);
            }

            onRead();
        });
}

void Network::onReadEndTurnMsg(std::size_t inbound_data_size)
{
    inbound_data_.resize(inbound_data_size);

    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(inbound_data_)),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            EndTurnMsg msg;
            try {
                std::string archive_data(&inbound_data_[0], inbound_data_.size());
                std::istringstream archiveStream(archive_data);
                boost::archive::text_iarchive archive(archiveStream);
                archive >> msg;
            } catch (std::exception& e) {
                // Unable to decode data.
                //              boost::system::error_code error(boost::asio::error::invalid_argument);
                //              boost::get<0>(handler)(error);
                return;
            }

            if (m_handleEnemyEndTurn) {
                m_handleEnemyEndTurn();
            }

            onRead();
        });
}

} // namespace PaperSoccer
