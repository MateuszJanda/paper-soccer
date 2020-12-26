# https://cmake.org/cmake/help/latest/module/FindProtobuf.html

## List generated sources files
FILE(GLOB HDRS "*.h")
FILE(GLOB SRCS "*.cc")

#set(PROTO_NAME proto_messages)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_MSG_SOURCE PROTO_MSG_HEADER Direction.proto Goal.proto Turn.proto MsgId.proto Message.proto)
add_library(PROTO_NAME ${PROTO_MSG_SOURCE} ${PROTO_MSG_HEADER})

target_link_libraries(PROTO_NAME ${Protobuf_LIBRARIES})

message(STATUS "[-]: ${PROTO_MSG_SOURCE}")
message(STATUS "[-]: ${PROTO_MSG_HEADER}")