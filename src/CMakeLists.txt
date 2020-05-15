# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    Board.cpp
    Board.hpp
)

# Create libpaper-soccer_lib.a
add_library(${CMAKE_PROJECT_NAME}_lib STATIC
    Board.cpp
    Board.hpp
)
