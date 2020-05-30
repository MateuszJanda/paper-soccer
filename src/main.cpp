#include "Server.hpp"
#include "Client.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "NCurses.hpp"
#include "Node.hpp"
#include "View.hpp"
#include <iostream>
#include <boost/asio.hpp>

using namespace std;

using boost::asio::ip::tcp;

//class Server
//{
//public:
//    Server(boost::asio::ioContext& ioContext, const tcp::endpoint& endpoint)
//        : m_acceptor(ioContext, endpoint),
//          m_socket(ioContext)
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



int main(int argc, char** argv)
{
    using namespace PaperSoccer;

    boost::asio::io_context ioContext;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard{ioContext.get_executor()};

//    std::shared_ptr<Server> server;
//    if (argc == 2 and std::string(argv[1]) == "-s")
//    {
//        std::cout << "server start" << "\n";
//        boost::asio::ip::tcp::endpoint endpoint{tcp::v4(), 8787};
//        server = std::make_shared<Server>(ioContext, endpoint);




//    }
//    else
//    {
//    }

//    std::cout << "thread before" << "\n";
//    std::thread t([&ioContext](){ ioContext.run(); });
//    t.join();
//    std::cout << "thread end" << "\n";


    Board b{8, 10};
    NCurses nn;
    View v{b, nn};
    v.drawBoard();

    Game game{b, nn, v};
    game.run2(ioContext);

    std::thread t([&ioContext](){ ioContext.run(); });
    t.join();
}
