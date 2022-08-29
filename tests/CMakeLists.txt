set(BINARY_NAME runTests)

# Protocol Buffer library
find_package(Protobuf REQUIRED)

# Set include dirs for googletest/googlemock
set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/googletest/googletest/include)
set(GMOCK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/include)

# Just for debug. Show full path
message("[+] googletest include path: " ${GTEST_INCLUDE_DIR})
message("[+] googlemock include path: " ${GTEST_INCLUDE_DIR})

include_directories(${GTEST_INCLUDE_DIR})
include_directories(${GMOCK_INCLUDE_DIR})

# QtCreator can use this
# Adds a test called <name>, <command> specifies an executable target
add_test(NAME ${BINARY_NAME} COMMAND ${BINARY_NAME})

add_executable(${BINARY_NAME}
    runTests.cpp
    GameTest.cpp
    BoardTest.cpp
    NodeTest.cpp
    PositionTest.cpp
    DirectionTest.cpp
    BoardMock.hpp
    NCursesMock.hpp
    NetworkMock.hpp
    TimerMock.hpp
    ViewMock.hpp
    ViewTest.cpp
    ViewBoardTest.cpp
    ViewMenuTest.cpp
)

link_directories(${CMAKE_SOURCE_DIR}/lib/googletest/lib)
message("[+] Lib path: " ${CMAKE_SOURCE_DIR}/lib/googletest/lib)

target_link_libraries(${BINARY_NAME}
    paper_soccer_lib
    paper_soccer_proto_lib
    gtest_main
    gtest
    gmock_main
    gmock
    pthread
    ${Protobuf_LIBRARIES}
)
