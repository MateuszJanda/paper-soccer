#include "Board.hpp"
#include <iostream>


template<std::size_t WIDTH, std::size_t HEIGHT>
Board<WIDTH, HEIGHT>::Board()
{
    std::cout << "adsf" << std::endl;
}

template<std::size_t WIDTH, std::size_t HEIGHT>
std::size_t Board<WIDTH, HEIGHT>::getWidth() const
{
    return this->graph[0].size() - WIDTH;
}

template<std::size_t WIDTH, std::size_t HEIGHT>
std::size_t Board<WIDTH, HEIGHT>::getHeight() const
{
    return this->graph[0].size() - HEIGHT;
}

template class Board<10, 8>;
