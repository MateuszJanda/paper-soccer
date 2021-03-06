// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Server.hpp"
#include <iostream>

namespace PaperSoccer {

Server::Server(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint)
    : Network{ioContext}
    , m_acceptor{ioContext, endpoint}
{
}

void Server::run()
{
    accept();
}

void Server::accept()
{
    std::cout << "Waiting for connection ...\n";
    m_acceptor.async_accept(m_socket,
        [this](boost::system::error_code errorCode) {
            if (not errorCode) {
                setupHandlers();

                if (m_handleInitNewGame) {
                    m_handleInitNewGame();
                }
            }
        });
}

} // namespace PaperSoccer
