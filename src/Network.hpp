#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "INetwork.hpp"
#include <boost/asio.hpp>
#include <deque>
#include <functional>

namespace PaperSoccer {

class Network : public INetwork {
public:
    Network(boost::asio::io_context& ioContext);

    void registerHandlers(std::function<void()> handleKeyboardMouseInput,
                          std::function<void()> handleInitNewGame,
                          std::function<void(const Turn&, const Goal&)> handleNewGame,
                          std::function<void(const Direction &)> handleEnemyMove) override;

    void onKeyboardMouseInput(boost::system::error_code errorCode);

    void sendNewGame(Turn turn, Goal goal) override;
    void sendMove(const Direction& dir) override;
    void sendEndTurn() override;
    void onWrite();

    void onReadHeader();
    void onReadNewGameMsg(std::size_t inbound_data_size);
    void onReadMoveMsg(std::size_t inbound_data_size);

protected:
    void setupHandlers();

    boost::asio::ip::tcp::socket m_socket;
    std::function<void()> m_handleInitNewGame;

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::posix::stream_descriptor m_desc;

    std::function<void()> m_handleKeyboardMouseInput;
    std::function<void(const Turn&, const Goal&)> m_handleNewGame;
    std::function<void(const Direction&)> m_handleEnemyMove;

    enum { header_length = 8 };
    enum { msgId_length = 8 };
    std::string outbound_msgId;
    /// Holds an outbound header.
    std::string outbound_header_;
    /// Holds the outbound data.
    std::string outbound_data_;

    char inbound_hhh[msgId_length + header_length];
    /// Holds an inbound header.
//    char inbound_header_[header_length];
    /// Holds the inbound data.
    std::vector<char> inbound_data_;

//    MoveMsg1 msg;
//    NewGameMsg newGameMsg;
//    std::deque<MoveMsg1> m_messageQueue;
    std::deque<boost::asio::const_buffer> m_messageQueue;
};

} // namespace PaperSoccer

#endif // NETWORK_HPP
