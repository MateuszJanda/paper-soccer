set(BINARY runTests)

set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCES_DIR}/Game.cpp
    ${SOURCES_DIR}/Board.cpp
    ${SOURCES_DIR}/Node.cpp
    ${SOURCES_DIR}/View.cpp
)

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
add_test(NAME ${BINARY} COMMAND ${BINARY})

add_executable(${BINARY}
    runTests.cpp
    GameTest.cpp
    BoardTest.cpp
    NodeTest.cpp
    PositionTest.cpp
    DirectionTest.cpp
    BoardMock.hpp
    NCursesMock.hpp
    NetworkMock.hpp
    ViewMock.hpp
    ViewTest.cpp
    ${SOURCES}
)

link_directories(${CMAKE_SOURCE_DIR}/lib/googletest/lib)
message("[+] Lib path: " ${CMAKE_SOURCE_DIR}/lib/googletest/lib)

target_link_libraries(${BINARY}
    gtest_main
    gtest
    gmock_main
    gmock
    pthread
)
