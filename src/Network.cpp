#include "Network.hpp"
#include "Messages.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <type_traits>
#include <memory>

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
    std::function<void(const Direction&)> handleEnemyMove)
{
    m_handleKeyboardMouseInput = handleKeyboardMouseInput;
    m_handleInitNewGame = handleInitNewGame;
    m_handleNewGame = handleNewGame;
    m_handleEnemyMove = handleEnemyMove;
}

void Network::setupHandlers()
{
    onReadHeader();
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
    // Write data
    NewGameMsg m{turn, goal};
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
        archive << m;
    outbound_data_ = archive_stream.str();

    // Write data size on 8 characters
    std::ostringstream msgId_stream;
    msgId_stream << std::setw(msgId_length) << std::hex << static_cast<std::underlying_type_t<MsgId>>(m.msgId);

    if (!msgId_stream || msgId_stream.str().size() != msgId_length)
    {
      // Something went wrong, inform the caller.
//      boost::system::error_code error(boost::asio::error::invalid_argument);
//      m_socket.io_service().post(boost::bind(handler, error));
      return;
    }

    outbound_msgId = msgId_stream.str();

    // Write data size on 8 characters
    std::ostringstream header_stream;
    header_stream << std::setw(header_length) << std::hex << outbound_data_.size();

    if (!header_stream || header_stream.str().size() != header_length)
    {
      // Something went wrong, inform the caller.
//      boost::system::error_code error(boost::asio::error::invalid_argument);
//      m_socket.io_service().post(boost::bind(handler, error));
      return;
    }

    outbound_header_ = header_stream.str();

//    std::vector<boost::asio::const_buffer> buffers;
//    buffers.push_back(boost::asio::buffer(outbound_header_));
//    buffers.push_back(boost::asio::buffer(outbound_data_));
//    boost::asio::async_write(socket_, buffers, handler);

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

void Network::sendMove(const Direction& dir)
{
    // Write data
    MoveMsg1 m{dir};
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
        archive << m;
    outbound_data_ = archive_stream.str();

    // Write data size on 8 characters
    std::ostringstream msgId_stream;
    msgId_stream << std::setw(msgId_length) << std::hex << static_cast<std::underlying_type_t<MsgId>>(m.msgId);

    if (!msgId_stream || msgId_stream.str().size() != msgId_length)
    {
      // Something went wrong, inform the caller.
//      boost::system::error_code error(boost::asio::error::invalid_argument);
//      m_socket.io_service().post(boost::bind(handler, error));
      return;
    }

    outbound_msgId = msgId_stream.str();

    // Write data size on 8 characters
    std::ostringstream header_stream;
    header_stream << std::setw(header_length) << std::hex << outbound_data_.size();

    if (!header_stream || header_stream.str().size() != header_length)
    {
      // Something went wrong, inform the caller.
//      boost::system::error_code error(boost::asio::error::invalid_argument);
//      m_socket.io_service().post(boost::bind(handler, error));
      return;
    }

    outbound_header_ = header_stream.str();

//    std::vector<boost::asio::const_buffer> buffers;
//    buffers.push_back(boost::asio::buffer(outbound_header_));
//    buffers.push_back(boost::asio::buffer(outbound_data_));
//    boost::asio::async_write(socket_, buffers, handler);

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

void Network::sendEndTurn()
{

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

void Network::onReadHeader()
{
    boost::asio::async_read(m_socket,
        boost::asio::buffer(boost::asio::buffer(inbound_hhh)),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
                m_socket.close();
                return;
            }

            // Determine the msgId
            std::istringstream is(std::string(inbound_hhh, msgId_length));
            std::uint8_t msgId = 0;
            if (!(is >> std::hex >> msgId))
            {
              return;
            }

            // Determine the length
            is = std::istringstream(std::string(inbound_hhh + header_length, header_length));
            std::size_t inbound_data_size = 0;
            if (!(is >> std::hex >> inbound_data_size))
            {
              return;
            }

            MsgId mmm = static_cast<MsgId>(msgId);
            if (mmm == MsgId::NewGame)
            {
                onReadNewGameMsg(inbound_data_size);
            }
            else if (mmm == MsgId::Move)
            {
                onReadMoveMsg(inbound_data_size);
            }

            return; // !!
        });
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
            try
            {
              std::string archive_data(&inbound_data_[0], inbound_data_.size());
              std::istringstream archive_stream(archive_data);
              boost::archive::text_iarchive archive(archive_stream);
              archive >> msg;
            }
            catch (std::exception& e)
            {
              // Unable to decode data.
//              boost::system::error_code error(boost::asio::error::invalid_argument);
//              boost::get<0>(handler)(error);
              return;
            }

            if (m_handleNewGame) {
                m_handleNewGame(msg.turn, msg.goal);
            }

            onReadHeader();
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

            MoveMsg1 msg;
            try
            {
              std::string archive_data(&inbound_data_[0], inbound_data_.size());
              std::istringstream archive_stream(archive_data);
              boost::archive::text_iarchive archive(archive_stream);
              archive >> msg;
            }
            catch (std::exception& e)
            {
              // Unable to decode data.
//              boost::system::error_code error(boost::asio::error::invalid_argument);
//              boost::get<0>(handler)(error);
              return;
            }

            if (m_handleEnemyMove) {
                m_handleEnemyMove(msg.dir);
            }

            onReadHeader();
        });
}

} // namespace PaperSoccer
