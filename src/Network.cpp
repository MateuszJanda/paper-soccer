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
    std::function<void(const Direction&)> handleEnemyMove)
{
    m_handleKeyboardMouseInput = handleKeyboardMouseInput;
    m_handleEnemyMove = handleEnemyMove;
}

void Network::onKeyboardMouseInput(boost::system::error_code errorCode)
{
    using namespace std::placeholders;

    if (not errorCode) {
        m_handleKeyboardMouseInput();
        m_desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read,
            std::bind(&Network::onKeyboardMouseInput, this, _1));
    }
}

void Network::onRead()
{
    boost::asio::async_read(m_socket,
        boost::asio::buffer(msg.data_, msg.length()),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (not errorCode and msg.decode()) {
                m_handleEnemyMove(msg.dir);
                rawPrint(0, 0, "ok read " + std::to_string((int)msg.dir) + "       ");
                onRead();
            } else {
//                rawPrint(0, 0, "error read " + std::to_string(counter) + "       ");
                //                cout << "read fail" << "\n";
                // m_socket.close(); ???
            }
        });
}


void Network::sendMove(const Direction& dir)
{
    boost::asio::post(m_ioContext,
        [this, dir]() {
            bool writeInProgress = !m_messageQueue.empty();
            m_messageQueue.push_back(TmpMoveMsg{dir});
            if (!writeInProgress) {
                onWrite();
            }
        });
}

void Network::sendEndTurn()
{

}

void Network::sendNewGame(Turn, UserGoal)
{

}

void Network::onWrite()
{
    boost::asio::async_write(m_socket,
        boost::asio::buffer(m_messageQueue.front().data(),
            m_messageQueue.front().length()),
        [this](boost::system::error_code errorCode, std::size_t length) {
            if (errorCode) {
//                rawPrint(0, 0, "error write " + std::to_string(counter) + "       ");
            } else {
//                rawPrint(0, 0, "write " + std::to_string(counter) + "       ");
            }

            m_messageQueue.pop_front();
            if (!m_messageQueue.empty()) {
//                rawPrint(0, 0, "write " + std::to_string(counter) + " " + std::to_string(length) + " ");
                onWrite();
            } else {
                //                m_socket.close(); ???
            }
        });
}

void Network::setupHandlers()
{
    onRead();
    onKeyboardMouseInput(boost::system::error_code{});
}

} // namespace PaperSoccer
