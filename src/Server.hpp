#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include "NCurses.hpp"
#include "INetwork.hpp"
#include <functional>
#include "TmpMoveMsg.hpp"
#include <deque>

namespace PaperSoccer {

class Server : public INetwork
{
public:
    Server(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint);

    void accept();
    void setupHandlers();

    void run(std::function<void()> handleKey, std::function<void(const TmpMoveMsg &)> handleR) override;
    void send(const TmpMoveMsg &msg) override;


    void onKeyboardMouseInput(boost::system::error_code errorCode);
    void onReadMsg();
    void onWrite();

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::posix::stream_descriptor m_desc;
    std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;

    std::function<void()> handleKeyboardMouseInput;
    std::function<void(const TmpMoveMsg&)> handleReadMsg;

    TmpMoveMsg msg;
    std::deque<TmpMoveMsg> m_messageQueue;
};

}

#endif // SERVER_HPP
