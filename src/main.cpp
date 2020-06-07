#include "Board.hpp"
#include "Client.hpp"
#include "Game.hpp"
#include "NCurses.hpp"
#include "Node.hpp"
#include "Server.hpp"
#include "TmpMoveMsg.hpp"
#include "View.hpp"
#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace PaperSoccer;

void runServer()
{
    using boost::asio::io_context;
    using boost::asio::ip::tcp;

    io_context ioContext;
    boost::asio::executor_work_guard<io_context::executor_type> guard{ioContext.get_executor()};

    tcp::endpoint endpoint{tcp::v4(), 8787};
    Server server{ioContext, endpoint};

    Board board{8, 10};
    NCurses ncurses;
    View view{board, ncurses};
    Game game{server, board, ncurses, view};
    game.run();

    // https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio/overview/core/threads.html
    // Asynchronous completion handlers will only be called from threads that are currently calling io_context::run().
    std::thread t([&ioContext]() { ioContext.run(); });
    t.join();
}

void runClient()
{
    using boost::asio::io_context;
    using boost::asio::ip::tcp;

    io_context ioContext;
    boost::asio::executor_work_guard<io_context::executor_type> guard{ioContext.get_executor()};

    tcp::resolver res{ioContext};
    auto endpoints = res.resolve("localhost", "8787");

    Client client{ioContext, endpoints};

    Board board{8, 10};
    NCurses ncurses;
    View view{board, ncurses};
    Game game{client, board, ncurses, view};
    game.run();

    std::thread t([&ioContext]() { ioContext.run(); });
    t.join();
}

int main(int argc, char** argv)
{
    if (argc == 2 and std::string(argv[1]) == "-s") {
        runServer();
    } else {
        runClient();
    }

    //    Board b{8, 10};
    //    NCurses nn;
    //    View v{b, nn};
    //    v.drawBoard();

    //    Game game{b, nn, v};
    //    game.run2(ioContext);

    //    std::thread t([&ioContext](){ ioContext.run(); });
    //    t.join();
}
