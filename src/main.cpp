#include "Server.hpp"
#include "Client.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "NCurses.hpp"
#include "Node.hpp"
#include "View.hpp"
#include "TmpMoveMsg.hpp"
#include <iostream>
#include <boost/asio.hpp>

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



    std::thread t([&ioContext](){ ioContext.run(); });
    t.join();
}

void runClient()
{
    using boost::asio::io_context;
    using boost::asio::ip::tcp;

    io_context ioContext;
    boost::asio::executor_work_guard<io_context::executor_type> guard{ioContext.get_executor()};

//    std::cout << "client start" << "\n";
    tcp::resolver res{ioContext};
    auto endpoints = res.resolve("localhost", "8787");

    Client client{ioContext, endpoints};

//    std::cout << "thread before" << "\n";
    std::thread t([&ioContext](){ ioContext.run(); });
    t.join();
}

int main(int argc, char** argv)
{
    if (argc == 2 and std::string(argv[1]) == "-s")
    {
        runServer();
    }
    else
    {
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
