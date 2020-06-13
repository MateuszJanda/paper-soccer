# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    NCurses.cpp
    NCurses.hpp
    Network.cpp
    Network.hpp
    Server.cpp
    Server.hpp
    Client.cpp
    Client.hpp
    Game.cpp
    Game.hpp
    Board.cpp
    Board.hpp
    Node.cpp
    Node.hpp
    Position.hpp
    Direction.hpp
    View.cpp
    View.hpp
    main.cpp
)

# Boost asio is header only
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})

# Boost system is required by Boost asio
find_package(Boost REQUIRED COMPONENTS system)
# Boost serialization
find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries(${CMAKE_PROJECT_NAME}
    pthread
    ncursesw
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
)
