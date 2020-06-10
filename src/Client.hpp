#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "INetwork.hpp"
#include "TmpMoveMsg.hpp"
#include <boost/asio.hpp>
#include <deque>
#include <functional>

namespace PaperSoccer {

class Client : public INetwork {
public:
    Client(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::resolver::results_type& endpoints);

    void connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void setupHandlers();

    void registerHandlers(std::function<void()> handleKeyboardMouseInput,
                          std::function<void(const TmpMoveMsg&)> handleMove) override;
    void run() override;
    void send(const TmpMoveMsg& msg) override;

    void onKeyboardMouseInput(boost::system::error_code errorCode);
    void onReadMsg();
    void onWrite();

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::socket m_socket;
    const boost::asio::ip::tcp::resolver::results_type& m_endpoints;
    boost::asio::posix::stream_descriptor m_desc;

    std::function<void()> m_handleKeyboardMouseInput;
    std::function<void(const TmpMoveMsg&)> m_handleMoveMsg;

    int counter{0};
    TmpMoveMsg msg;
    std::deque<TmpMoveMsg> m_messageQueue;
};

} // namespace PaperSoccer

#endif // CLIENT_HPP
