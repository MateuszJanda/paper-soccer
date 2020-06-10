#ifndef SERVER_HPP
#define SERVER_HPP

#include "INetwork.hpp"
#include "NCurses.hpp"
#include "TmpMoveMsg.hpp"
#include <boost/asio.hpp>
#include <deque>
#include <functional>

namespace PaperSoccer {

class Server : public INetwork {
public:
    Server(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint);

    void accept();
    void setupHandlers();

    void registerHandlers(std::function<void()> handleKeyboardMouseInput,
                          std::function<void(const TmpMoveMsg&)> handleMove) override;
    void run() override;
    void send(const TmpMoveMsg& msg) override;

    void onKeyboardMouseInput(boost::system::error_code errorCode);
    void onRead();
    void onWrite();

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::posix::stream_descriptor m_desc;

    std::function<void()> m_handleKeyboardMouseInput;
    std::function<void(const TmpMoveMsg&)> m_handleMoveMsg;

    int counter{0};
    TmpMoveMsg msg;
    std::deque<TmpMoveMsg> m_messageQueue;
};

} // namespace PaperSoccer

#endif // SERVER_HPP
