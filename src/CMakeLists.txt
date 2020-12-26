include(FindProtobuf)
find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})


# Add Protobuf message definition
add_subdirectory(proto)


# Just for debug. Show full path
message(STATUS "[+]: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "[+]: ${Protobuf_LIBRARIES}")
message(STATUS "[+]: ${PROTO_MSG_SOURCE}")
message(STATUS "[+]: ${PROTO_MSG_HEADER}")
message(STATUS "[+]: ${HDRS}")
message(STATUS "[+]: ${SRCS}")


# Boost asio is header only
include_directories(${Boost_INCLUDE_DIR})

include_directories("proto")

# Boost system is required by Boost asio
find_package(Boost REQUIRED COMPONENTS system)
# Boost program_options (parse command line parameters)
find_package(Boost REQUIRED COMPONENTS program_options)
# Boost serialization
find_package(Boost REQUIRED COMPONENTS serialization)

# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    ColorPair.hpp
    IBoard.hpp
    INCurses.hpp
    IView.hpp
    INetwork.hpp
    ITimer.hpp
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

target_link_libraries(${CMAKE_PROJECT_NAME}
    pthread
    ncursesw
    proto_messages
    ${Protobuf_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
