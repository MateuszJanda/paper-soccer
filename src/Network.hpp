#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "INetwork.hpp"
#include "Messages.hpp"
#include <boost/asio.hpp>
#include <deque>
#include <functional>

namespace PaperSoccer {

class Network : public INetwork {
public:
    Network(boost::asio::io_context& ioContext);

    void registerHandlers(std::function<void()> handleKeyboardMouseInput,
        std::function<void()> handleInitNewGame,
        std::function<void(NewGameMsg)> handleNewGame,
        std::function<void(const Direction&)> handleEnemyMove,
        std::function<void()> handleEnemyEndTurn) override;

    void onKeyboardMouseInput(boost::system::error_code errorCode);

    void sendNewGame(Turn turn, Goal goal) override;
    void sendMove(const Direction& dir) override;
    void sendEndTurn() override;

    template<typename Msg>
    void sendMsg(const Msg& msg);
    std::string encodeMsgId(MsgId msgId);
    template<typename Msg>
    std::string encodeData(const Msg& msg);
    std::string encodeDataSize(const std::string& data);
    void onWrite();

    void onRead();
    MsgId decodeMsgId(const std::string& inboundData);
    std::size_t decodeDataSize(const std::string& data);

//    void onReadNewGameMsg(std::size_t inbound_data_size);

    template<typename Msg>
    void onReadMsg(std::size_t dataSize, std::function<void(Msg)> handlerFunc);

    void onReadMoveMsg(std::size_t inbound_data_size);
    void onReadEndTurnMsg(std::size_t inbound_data_size);

protected:
    void setupHandlers();

    boost::asio::ip::tcp::socket m_socket;
    std::function<void()> m_handleInitNewGame;

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::posix::stream_descriptor m_desc;

    std::function<void()> m_handleKeyboardMouseInput;
    std::function<void(NewGameMsg)> m_handleNewGame;
    std::function<void(const Direction&)> m_handleEnemyMove;
    std::function<void()> m_handleEnemyEndTurn;

    static constexpr int MSG_ID_LENGTH{8};
    static constexpr int DATA_SIZE_LENGTH{8};

    std::string outbound_msgId;
    /// Holds an outbound header.
    std::string outbound_header_;
    /// Holds the outbound data.
    std::string outbound_data_;

    char inbound_hhh[MSG_ID_LENGTH + DATA_SIZE_LENGTH];
    /// Holds an inbound header.
    //    char inbound_header_[header_length];
    /// Holds the inbound data.
    std::vector<char> inbound_data_;

    //    MoveMsg msg;
    //    NewGameMsg newGameMsg;
    //    std::deque<MoveMsg> m_messageQueue;
    std::deque<boost::asio::const_buffer> m_messageQueue;
};

} // namespace PaperSoccer

#endif // NETWORK_HPP
