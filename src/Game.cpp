#include "Game.hpp"
#include <iostream>
#include <thread>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/asio/posix/descriptor.hpp>


namespace PaperSoccer {

using boost::asio::ip::tcp;

//class Server
//{
//public:
//    Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
//        : m_acceptor(io_context, endpoint),
//          m_socket(io_context)
//    {
//        accept();
//    }

//    void accept()
//    {
//        m_acceptor.async_accept(m_socket,
//            [this](boost::system::error_code errorCode)
//            {
//                if (not errorCode)
//                {
////                    std::make_shared<chat_session>(std::move(m_socket), room_)->start();
//                    std::cout << "accept" << "\n";
//                }
//            });
//    }


//private:
//    tcp::acceptor m_acceptor;
//    tcp::socket m_socket;
//};


Game::Game(IBoard& board, INCurses& ncurses, View& view)
  : m_board{board}
  , m_ncurses{ncurses}
  , m_view{view}
{

}

void Game::run()
{
    int i =0;
    while(1)
    {
        auto [c, x, y] = m_ncurses.getChar();
        ddd(c, x, y);
        m_view.printText(0, 1, std::to_string(i));
        i++;
    }
//    m_view.printText(0, 1, "exit loop");
}

void Game::ddd(int c, int x, int y)
{
    switch(c) {
    case 'q':
        m_board.moveBall(Direction::TopLeft);
        m_view.drawBoard();
        break;
    case 'w':
        m_board.moveBall(Direction::Top);
        m_view.drawBoard();
        break;
    case 'e':
        m_board.moveBall(Direction::TopRight);
        m_view.drawBoard();
        break;
    case 'a':
        m_board.moveBall(Direction::Left);
        m_view.drawBoard();
        break;
    case 'd':
        m_board.moveBall(Direction::Right);
        m_view.drawBoard();
        break;
    case 'z':
        m_board.moveBall(Direction::BottomLeft);
        m_view.drawBoard();
        break;
    case 'x':
        m_board.moveBall(Direction::Bottom);
        m_view.drawBoard();
        break;
    case 'c':
        m_board.moveBall(Direction::BottomRight);
        m_view.drawBoard();
        break;
    }

    if (c == 1000)
    {
        std::stringstream sss;
        sss << "Mouse " << x << " " << y << "                             ";
        m_view.printText(0, 1, sss.str());
    }
}

void Game::on_input()
{
    std::stringstream ss;
//    ss << "OnLoop: " << std::this_thread::get_id();
    m_view.printText(0, 1, ss.str());

    while(1)
    {
        auto [c, x, y] = m_ncurses.getChar();
        m_view.printText(0, 1, std::to_string(c));
        if (c == /*ERR*/ -1)
                break;
        ddd(c, x, y);
    }

}


void Game::run1()
{
    std::stringstream ss;
    ss << "Main:   " << std::this_thread::get_id();
    m_view.printText(0, 0, ss.str());

    std::function<void(boost::system::error_code)> input_loop;

    // https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio/overview/core/threads.html
    // Asynchronous completion handlers will only be called from threads that are currently calling io_context::run().
    boost::asio::io_context io_context;
    boost::asio::posix::stream_descriptor d(io_context, 0);
    input_loop = [&, this](boost::system::error_code ec) {
        if (!ec) {
            on_input();
            d.async_wait(boost::asio::posix::descriptor::wait_type::wait_read, input_loop);
        }
    };

//    while(1)
//    {
//        int c = m_ncurses.getChar();

//    }
    input_loop(boost::system::error_code{});

    m_view.printText(0, 0, "after loop");

    using namespace std::literals;


//    io_context.run_for(10s);
    std::thread t([&io_context](){ io_context.run(); });
    t.join();
}

}
