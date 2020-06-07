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

    void run(std::function<void()> handleKey, std::function<void(const TmpMoveMsg&)> handleR) override;
    void send(const TmpMoveMsg& msg) override;

    void onKeyboardMouseInput(boost::system::error_code errorCode);
    void onReadMsg();
    void onWrite();

private:
    boost::asio::io_context& m_ioContext;
    const boost::asio::ip::tcp::resolver::results_type& m_endpoints;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::posix::stream_descriptor m_desc;

    std::function<void()> handleKeyboardMouseInput;
    std::function<void(const TmpMoveMsg&)> handleReadMsg;

    int counter{0};
    TmpMoveMsg msg;
    std::deque<TmpMoveMsg> m_messageQueue;
};

}

#endif // CLIENT_HPP
