#include "Network.hpp"
#include "NCurses.hpp"
#include <memory>

namespace PaperSoccer {

Network::Network(boost::asio::io_context& ioContext)
    : m_ioContext{ioContext}
    , m_socket{ioContext}
    , m_desc{ioContext, 0}
    , msg{Direction::Top}
{
}

void Network::registerHandlers(std::function<void()> handleKeyboardMouseInput,
    std::function<void()> handleInitNewGame,
    std::function<void(const Direction&)> handleEnemyMove)
{
    m_handleKeyboardMouseInput = handleKeyboardMouseInput;
    m_handleEnemyMove = handleEnemyMove;
    m_handleInitNewGame = m_handleInitNewGame;
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

void Network::onRead()
{
    boost::asio::async_read(m_socket,
        boost::asio::buffer(msg.data_, msg.length()),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode or msg.decode()) {
                m_socket.close();
                return;
            }

            if (m_handleEnemyMove) {
                m_handleEnemyMove(msg.dir);
            }

            onRead();
        });
}

void Network::onWrite()
{
    boost::asio::async_write(m_socket,
        boost::asio::buffer(m_messageQueue.front().data(),
            m_messageQueue.front().length()),
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

void Network::sendMove(const Direction& dir)
{
    boost::asio::post(m_ioContext,
        [this, dir]() {
            bool writeInProgress = not m_messageQueue.empty();
            m_messageQueue.push_back(MoveMsg{dir});
            if (not writeInProgress) {
                onWrite();
            }
        });
}

void Network::sendEndTurn()
{

}

void Network::sendNewGame(Turn, Goal)
{

}

} // namespace PaperSoccer
