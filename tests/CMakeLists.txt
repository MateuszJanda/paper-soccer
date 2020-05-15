cmake_minimum_required(VERSION 3.5)

project(runTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(runTests runTests.cpp BoardTest.cpp)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/libgtest.a
    ${CMAKE_SOURCE_DIR}/libgtest_main.a
    pthread
)
