# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    Board.cpp
    Board.hpp
    Node.cpp
    Node.hpp
    main.cpp
)

# Create libpaper-soccer_lib.a
add_library(${CMAKE_PROJECT_NAME}_lib STATIC
    Board.cpp
    Board.hpp
    Node.cpp
    Node.hpp
)
