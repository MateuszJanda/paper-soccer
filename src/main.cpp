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



int main(int argc, char** argv)
{
    using namespace PaperSoccer;

//    try
//    {
//        cout << "Server start" << "\n";
////        boost::asio::io_service io_service;
//        boost::asio::io_context io_context;

//        tcp::endpoint endpoint(tcp::v4(), 8787);

//        Server server{io_context, endpoint};

////        io_service.run();
//        io_context.run();
//    }
//    catch (std::exception& e)
//    {
//        std::cerr << "Exception: " << e.what() << "\n";
//    }

//    cout << "Exit" << "\n";

    Board b{8, 10};

    NCurses nn;
//    nn.getChar();
    View v{b, nn};
    v.drawBoard();

    Game game{b, nn, v};
    game.run();
}
