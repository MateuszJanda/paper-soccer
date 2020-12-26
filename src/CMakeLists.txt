# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    ColorPair.hpp
    IBoard.hpp
    INCurses.hpp
    IView.hpp
    INetwork.hpp
    ITimer.hpp
    Goal.pb.cc
    Goal.pb.h
    Direction.pb.h
    Direction.pb.cc
    Turn.pb.h
    Turn.pb.cc
    MsgId.pb.h
    MsgId.pb.cc
    Message.pb.h
    Message.pb.cc
    NCurses.cpp
    NCurses.hpp
    Timer.hpp
    Timer.cpp
    Network.hpp
    Network.cpp
    Server.hpp
    Server.cpp
    Client.hpp
    Client.cpp
    Game.hpp
    Game.cpp
    Board.hpp
    Board.cpp
    Node.hpp
    Node.cpp
    Position.hpp
    DirectionUtils.hpp
    View.hpp
    View.cpp
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
    protobuf
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
