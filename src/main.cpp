#include "Game.hpp"
#include "Board.hpp"
#include "NCurses.hpp"
#include "Node.hpp"
#include "View.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    using namespace PaperSoccer;
    Board b{8, 10};

    cout << "⸝ᐟᐠ৲" << endl;
    cout << "><" << endl;

    NCurses nn;
//    nn.getChar();
    View v{b, nn};
    v.drawBoard();

    Game game{b, nn, v};
    game.run();
}
