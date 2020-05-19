set(BINARY runTests)

set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCES_DIR}/Board.cpp
    ${SOURCES_DIR}/Node.cpp
)


# QtCreator can use this
# Adds a test called <name>, <command> specifies an executable target
add_test(NAME ${BINARY} COMMAND ${BINARY})

add_executable(${BINARY}
    runTests.cpp
    BoardTest.cpp
    NodeTest.cpp
    PositionTest.cpp
    DirectionTest.cpp
    ${SOURCES}
)

target_link_libraries(${BINARY}
    libgtest.a
    libgtest_main.a
    pthread
)
