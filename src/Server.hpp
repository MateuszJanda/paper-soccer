// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Network.hpp"
#include <boost/asio.hpp>

namespace PaperSoccer {

class Server : public Network {
public:
    Server(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint);

    void run() override;
    void accept();

private:
    boost::asio::ip::tcp::acceptor m_acceptor;
};

} // namespace PaperSoccer

#endif // SERVER_HPP
