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

    NCurses nn;
    View v{b, nn};
    v.drawBoard();
    nn.wait();
}
