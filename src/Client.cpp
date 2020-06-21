// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Client.hpp"

namespace PaperSoccer {

Client::Client(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::resolver::results_type& endpoints)
    : Network{ioContext}
    , m_endpoints{endpoints}
{
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

} // namespace PaperSoccer
