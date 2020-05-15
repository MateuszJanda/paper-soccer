cmake_minimum_required(VERSION 3.5)

project(test-paper-soccer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(paper-soccer mainTest.cpp BoardTest.cpp)
