# Protocol Buffer library
find_package(Protobuf REQUIRED)
# Boost system is required by Boost asio
find_package(Boost REQUIRED COMPONENTS system)
# Boost program_options (parse command line parameters)
find_package(Boost REQUIRED COMPONENTS program_options)

# Include directories for Google Protocol Buffers
include_directories(${Protobuf_INCLUDE_DIRS})

# Just for debug
message(STATUS "[+] Just for debug Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")

# Generate protobuf messages from definitions
add_subdirectory(proto)
# Add directory with protobuf messages (headers) to those the compiler uses to search for include files
include_directories("proto")

# Boost asio is header only
include_directories(${Boost_INCLUDE_DIR})

# Create paper-soccer lib (used also by runTests)
set(PROJECT_LIB paper_soccer_lib)
add_library(${PROJECT_LIB}
    NCurses.cpp
    Timer.cpp
    Network.cpp
    Server.cpp
    Client.cpp
    Game.cpp
    Board.cpp
    Node.cpp
    View.cpp
    ViewBoard.cpp
)

# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${PROJECT_LIB}
    pthread
    ncursesw
    paper_soccer_proto
    ${Protobuf_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
