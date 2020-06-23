set(BINARY runTests)

set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCES_DIR}/Game.cpp
    ${SOURCES_DIR}/Board.cpp
    ${SOURCES_DIR}/Node.cpp
    ${SOURCES_DIR}/View.cpp
)


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

target_link_libraries(${BINARY}
    libgtest.a
    libgtest_main.a
    libgmock.a
    libgmock_main.a
    pthread
)
