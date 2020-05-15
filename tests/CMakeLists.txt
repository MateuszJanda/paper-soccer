set(BINARY runTests)
set(SOURCES ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})
add_executable(${BINARY} runTests.cpp BoardTest.cpp  ${TEST_SOURCES})

target_link_libraries(${BINARY}
    libgtest.a
    libgtest_main.a
    pthread
)
