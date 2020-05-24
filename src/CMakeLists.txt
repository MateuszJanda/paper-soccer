# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    Game.cpp
    Game.hpp
    Board.cpp
    Board.hpp
    Node.cpp
    Node.hpp
    Position.hpp
    Direction.hpp
    NCurses.cpp
    NCurses.hpp
    View.cpp
    View.hpp
    main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ncurses
)
