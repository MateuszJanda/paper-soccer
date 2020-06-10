#include "Client.hpp"
#include "NCurses.hpp"

namespace PaperSoccer {

Client::Client(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::resolver::results_type& endpoints)
    : m_ioContext{ioContext}
    , m_socket{ioContext}
    , m_endpoints{endpoints}
    , m_desc{ioContext, 0}
    , msg{Direction::Top}
{
}

void Client::registerHandlers(std::function<void()> handleKeyboardMouseInput,
    std::function<void(const TmpMoveMsg&)> handleMove)
{
    m_handleKeyboardMouseInput = handleKeyboardMouseInput;
    m_handleMoveMsg = handleMove;
}

void Client::run()
{
    connect(m_endpoints);
}

void Client::connect(const boost::asio::ip::tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, endpoints,
        [this](boost::system::error_code errorCode, boost::asio::ip::tcp::endpoint) {
            if (not errorCode) {
                setupHandlers();
            }
        });
}

void Client::onKeyboardMouseInput(boost::system::error_code errorCode)
{
    using namespace std::placeholders;

    if (not errorCode) {
        m_handleKeyboardMouseInput();
        m_desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read,
            std::bind(&Client::onKeyboardMouseInput, this, _1));
    }
}

void Client::onReadMsg()
{
    boost::asio::async_read(m_socket,
        boost::asio::buffer(msg.data_, msg.length()),
        [&, this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec && msg.decode()) {
                m_handleMoveMsg(msg);
                onReadMsg();
            } else {
                // m_socket.close(); ???
            }
        });
}

void Client::send(const TmpMoveMsg& msg)
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

void Client::onWrite()
{
    boost::asio::async_write(m_socket,
        boost::asio::buffer(m_messageQueue.front().data(),
            m_messageQueue.front().length()),
        [this](boost::system::error_code ec, std::size_t length) {
            counter++;
            if (ec) {
                rawPrint(0, 0, "error write " + std::to_string(counter) + "       ");
            } else {
                //                rawPrint(0, 0, "write " + std::to_string(counter) + "       ");
                rawPrint(0, 0, "write " + std::to_string(m_socket.is_open()) + "       ");
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

void Client::setupHandlers()
{
    onReadMsg();
    onKeyboardMouseInput(boost::system::error_code{});
}

} // namespace PaperSoccer
