#include "Board.hpp"
#include <iostream>


template<std::size_t WIDTH, std::size_t HEIGHT>
Board<WIDTH, HEIGHT>::Board()
{
    std::cout << "adsf" << std::endl;
}

template class Board<10, 8>;
