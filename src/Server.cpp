#include "Server.hpp"
#include <iostream>
#include <memory>
#include <functional>

namespace PaperSoccer {

Server::Server(boost::asio::io_context &ioContext, const boost::asio::ip::tcp::endpoint& endpoint)
    : m_ioContext{ioContext},
      m_acceptor{ioContext, endpoint},
      m_desc{ioContext, 0},
      msg{Direction::Top}
{
}

void Server::run(std::function<void()> handleKey, std::function<void(const TmpMoveMsg &)> handleR)
{
    handleKeyboardMouseInput = handleKey;
    handleReadMsg = handleR;
    accept();
}

void Server::accept()
{
    m_socket = std::make_shared<boost::asio::ip::tcp::socket>(m_ioContext);
    m_acceptor.async_accept(
        [this](boost::system::error_code errorCode, boost::asio::ip::tcp::socket socket)
        {
            if (not errorCode) {
                setupHandlers();
            }
        });
}

void Server::onKeyboardMouseInput(boost::system::error_code errorCode)
{
    using namespace std::placeholders;

    if (not errorCode) {
        handleKeyboardMouseInput();
        m_desc.async_wait(boost::asio::posix::descriptor::wait_type::wait_read,
                          std::bind(&Server::onKeyboardMouseInput, this, _1));
    }
}

void Server::onReadMsg()
{
    boost::asio::async_read(*m_socket,
        boost::asio::buffer(msg.data_, msg.length()),
        [&, this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec && msg.decode())
            {
                handleReadMsg(msg);
            }
            else
            {
                // m_socket.close(); ???
            }
        });
}

void Server::send(const TmpMoveMsg& msg)
{
  boost::asio::post(m_ioContext,
      [this, msg]()
      {
        bool write_in_progress = !m_messageQueue.empty();
        m_messageQueue.push_back(msg);
        if (!write_in_progress)
        {
          onWrite();
        }
      });
}

void Server::onWrite()
{
    boost::asio::async_write(*m_socket,
        boost::asio::buffer(m_messageQueue.front().data(),
                            m_messageQueue.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            m_messageQueue.pop_front();
            if (!m_messageQueue.empty())
            {
              onWrite();
            }
            else
            {
//                m_socket.close(); ???
            }
        });
}

void Server::setupHandlers()
{
    onReadMsg();
    onKeyboardMouseInput(boost::system::error_code{});
}

}
