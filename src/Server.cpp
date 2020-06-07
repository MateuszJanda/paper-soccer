#include "Server.hpp"
#include "NCurses.hpp"
#include <functional>
#include <iostream>
#include <memory>

namespace PaperSoccer {

Server::Server(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint)
    : m_ioContext{ioContext}
    , m_socket{ioContext}
    , m_acceptor{ioContext, endpoint}
    , m_desc{ioContext, 0}
    , msg{Direction::Top}
{
}

void Server::registerHandlers(std::function<void()> handleKey,
                      std::function<void(const TmpMoveMsg&)> handleMoveMsg)
{
    m_handleKeyboardMouseInput = handleKey;
    m_handleMoveMsg = handleMoveMsg;
}

void Server::run()
{
    accept();
}

void Server::accept()
{
    m_acceptor.async_accept(m_socket,
        [this](boost::system::error_code errorCode) {
            if (not errorCode) {
                setupHandlers();
            }
        });
}

void Server::onKeyboardMouseInput(boost::system::error_code errorCode)
{
    using namespace std::placeholders;

    if (not errorCode) {
        m_handleKeyboardMouseInput();
        m_desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read,
            std::bind(&Server::onKeyboardMouseInput, this, _1));
    }
}

void Server::onReadMsg()
{
    boost::asio::async_read(m_socket,
        boost::asio::buffer(msg.data_, msg.length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            counter++;
            if (!ec && msg.decode()) {
                m_handleMoveMsg(msg);
                rawPrint(0, 0, "ok read " + std::to_string((int)msg.dir) + "       ");
                onReadMsg();
            } else {
                rawPrint(0, 0, "error read " + std::to_string(counter) + "       ");
                //                cout << "read fail" << "\n";
                // m_socket.close(); ???
            }
        });
}

void Server::send(const TmpMoveMsg& msg)
{
    boost::asio::post(m_ioContext,
        [this, msg]() {
            bool write_in_progress = !m_messageQueue.empty();
            m_messageQueue.push_back(msg);
            if (!write_in_progress) {
                onWrite();
            }
        });
}

void Server::onWrite()
{
    boost::asio::async_write(m_socket,
        boost::asio::buffer(m_messageQueue.front().data(),
            m_messageQueue.front().length()),
        [this](boost::system::error_code ec, std::size_t length) {
            counter++;
            if (ec) {
                rawPrint(0, 0, "error write " + std::to_string(counter) + "       ");
            } else {
                rawPrint(0, 0, "write " + std::to_string(counter) + "       ");
            }

            m_messageQueue.pop_front();
            if (!m_messageQueue.empty()) {
                rawPrint(0, 0, "write " + std::to_string(counter) + " " + std::to_string(length) + " ");
                onWrite();
            } else {
                //                m_socket.close(); ???
            }
        });
}

void Server::setupHandlers()
{
    onReadMsg();
    onKeyboardMouseInput(boost::system::error_code{});
}

} // namespace PaperSoccer
