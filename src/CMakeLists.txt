# Protocol Buffer library
find_package(Protobuf REQUIRED)
# Boost system is required by Boost asio
find_package(Boost REQUIRED COMPONENTS system)
# Boost program_options (parse command line parameters)
find_package(Boost REQUIRED COMPONENTS program_options)

# Just for debug
message(STATUS "[+] Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "[+] Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
message(STATUS "[+] Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")


# Include directories for Google Protocol Buffers
include_directories(${Protobuf_INCLUDE_DIRS})
# Boost asio is header only
include_directories(${Boost_INCLUDE_DIR})
# Include folder with protobuf headers
include_directories(proto)

# Generate protobuf messages from definitions
message(STATUS "[!] .----------------------------------------------------------.")
message(STATUS "[!] | Make sure you have installed protoc (protobuf-compiler)! |")
message(STATUS "[!] `----------------------------------------------------------`")
message(STATUS "[+] execute protc in ${CMAKE_SOURCE_DIR}/src/proto")
set(PAPER_SOCCER_PROTO_FILES
    Direction.proto
    Goal.proto
    Turn.proto
    MsgId.proto
    Message.proto
)

# It's better to just execute protoc than use protobuf_generate_cpp
execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/proto COMMAND protoc --cpp_out=. ${PAPER_SOCCER_PROTO_FILES})

# This is terrible feature :( I leave this just for legacy.
# protobuf_generate_cpp(PAPER_SOCCER_PROTO_MSG_SOURCE PAPER_SOCCER_PROTO_MSG_HEADER
#     proto/Direction.proto
#     proto/Goal.proto
#     proto/Turn.proto
#     proto/MsgId.proto
#     proto/Message.proto
# )

# Create paper-soccer protobuf lib (used also by runTests)
set(PAPER_SOCCER_PROTO_LIB paper_soccer_proto_lib)
add_library(${PAPER_SOCCER_PROTO_LIB}
    proto/Direction.pb.cc
    proto/Goal.pb.cc
    proto/Turn.pb.cc
    proto/MsgId.pb.cc
    proto/Message.pb.cc
)

# Create paper-soccer lib (used also by runTests)
set(PAPER_SOCCER_PROJECT_LIB paper_soccer_lib)
add_library(${PAPER_SOCCER_PROJECT_LIB}
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
    ViewMenu.cpp
)

# Create paper-soccer exe
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${PAPER_SOCCER_PROJECT_LIB}
    ${PAPER_SOCCER_PROTO_LIB}
    pthread
    ncursesw
    ${Protobuf_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
