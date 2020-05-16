set(BINARY runTests)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/Board.cpp
    ${CMAKE_SOURCE_DIR}/src/Node.cpp
)

# QtCreator can use this
# Adds a test called <name>, <command> specifies an executable target
add_test(NAME ${BINARY} COMMAND ${BINARY})

add_executable(${BINARY}
    runTests.cpp
    BoardTest.cpp
    ${SOURCES}
)

target_link_libraries(${BINARY}
    libgtest.a
    libgtest_main.a
    pthread
)
