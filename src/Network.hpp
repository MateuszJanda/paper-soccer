#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "INetwork.hpp"
#include "TmpMoveMsg.hpp"
#include <boost/asio.hpp>
#include <deque>
#include <functional>

namespace PaperSoccer {

class Network : public INetwork {
public:
    Network(boost::asio::io_context& ioContext);

    void setupHandlers();

    void registerHandlers(std::function<void()> handleKeyboardMouseInput,
                          std::function<void(const Direction &)> handleEnemyMove) override;
    void sendMove(const Direction& dir) override;
    void sendEndTurn() override;
    void sendNewGame(Turn, UserGoal) override;

    void onKeyboardMouseInput(boost::system::error_code errorCode);
    void onRead();
    void onWrite();

protected:
    boost::asio::ip::tcp::socket m_socket;

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::posix::stream_descriptor m_desc;

    std::function<void()> m_handleKeyboardMouseInput;
    std::function<void(const Direction&)> m_handleEnemyMove;

    TmpMoveMsg msg;
    std::deque<TmpMoveMsg> m_messageQueue;
};

} // namespace PaperSoccer

#endif // NETWORK_HPP
