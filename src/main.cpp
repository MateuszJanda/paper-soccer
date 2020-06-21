// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Board.hpp"
#include "Client.hpp"
#include "Game.hpp"
#include "NCurses.hpp"
#include "Node.hpp"
#include "Server.hpp"
#include "View.hpp"
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <iostream>

using namespace PaperSoccer;

boost::program_options::options_description usage()
{
    namespace po = boost::program_options;

    po::options_description desc(
                "\n"
                "paper-soccer version 0.99 (C) 2020  Mateusz Janda <mateusz.janda at gmail.com>"
                "\n\n"
                "Options");
    desc.add_options()
        ("help,h", "display this help")
        ("wait,w", "run as server, wait for connection")
    ;

    return desc;
}

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

int main(int argc, char* argv[])
{
    namespace po = boost::program_options;

    auto desc = usage();

    try {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("wait")) {
            runServer();
        } else {
            runClient();
        }
    } catch (po::too_many_positional_options_error &e) {
        std::cerr << e.what() << "\n";
        std::cout << desc << "\n";
        exit(1);
    } catch (po::error_with_option_name &e) {
        std::cerr << e.what() << "\n";
        std::cout << desc << "\n";
        exit(1);
    }

    return 0;
}
