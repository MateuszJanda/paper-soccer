# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    ColorPair.hpp
    Goal.hpp
    IBoard.hpp
    INCurses.hpp
    IView.hpp
    INetwork.hpp
    ITimer.hpp
    Messages.hpp
    Turn.hpp
    NCurses.cpp
    NCurses.hpp
    Timer.cpp
    Timer.hpp
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

# Boost system is required by Boost asio
find_package(Boost REQUIRED COMPONENTS system)
# Boost program_options (parse command line parameters)
find_package(Boost REQUIRED COMPONENTS program_options)
# Boost serialization
find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries(${CMAKE_PROJECT_NAME}
    pthread
    ncursesw
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
