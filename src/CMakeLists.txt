# Protocol Buffer library
find_package(Protobuf REQUIRED)
# Boost system is required by Boost asio
find_package(Boost REQUIRED COMPONENTS system)
# Boost program_options (parse command line parameters)
find_package(Boost REQUIRED COMPONENTS program_options)
# Boost serialization
find_package(Boost REQUIRED COMPONENTS serialization)

# Include directories for Google Protocol Buffers
include_directories(${Protobuf_INCLUDE_DIRS})

# Just for debug
message(STATUS "[+] Just for debug Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")

# Generate protobuf messages from definitions
add_subdirectory(proto)
# Add directory with protobuf messages (header)to those the compiler uses to search for include files
include_directories("proto")

# Boost asio is header only
include_directories(${Boost_INCLUDE_DIR})

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
    paper_soccer_proto
    ${Protobuf_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
