#include "Game.hpp"
#include <iostream>
#include <thread>
#include <sstream>
#include <TmpMoveMsg.hpp>
#include <boost/asio.hpp>
#include <boost/asio/posix/descriptor.hpp>


namespace PaperSoccer {

using boost::asio::ip::tcp;

Game::Game(IBoard& board, INCurses& ncurses, View& view, tcp::socket socket)
  : m_board{board}
  , m_ncurses{ncurses}
  , m_view{view}
  , m_socket{std::move(socket)}
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

Direction Game::direct(int c)
{
    switch(c) {
    case 'q':
        return Direction::TopLeft;
    case 'w':
        return Direction::Top;
    case 'e':
        return Direction::TopRight;
    case 'a':
        return Direction::Left;
    case 'd':
        return Direction::Right;
    case 'z':
        return Direction::BottomLeft;
    case 'x':
        return Direction::Bottom;
    case 'c':
        return Direction::BottomRight;
    }

    return Direction::Top;
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
    } else if (c != -1) {
        Direction dir = direct(c);
        send(dir);
    }
}

void Game::send(Direction dir)
{
    TmpMoveMsg msg{dir};

    boost::asio::async_write(m_socket,
        boost::asio::buffer(msg.data(), msg.length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (ec)
            {
                m_socket.close();
            }
        });
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

void Game::run2(boost::asio::io_context& ioContext)
{
    dxx = std::make_shared<boost::asio::posix::stream_descriptor>(ioContext, 0);
    input_loop = [&, this](boost::system::error_code ec) {
        if (!ec) {
            on_input();
            dxx->async_wait(boost::asio::posix::descriptor::wait_type::wait_read, input_loop);
        }
        std::cout << "run2 error" << std::endl;
    };

    input_loop(boost::system::error_code{});
//    std::thread t([&ioContext](){ ioContext.run(); });
//    t.join();
}

void Game::readInitMsg()
{

}

}
