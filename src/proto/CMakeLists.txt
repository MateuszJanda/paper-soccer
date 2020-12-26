# https://cmake.org/cmake/help/latest/module/FindProtobuf.html

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_MSG_SOURCE PROTO_MSG_HEADER Direction.proto Goal.proto Turn.proto MsgId.proto Message.proto)
add_library(proto_messages ${PROTO_MSG_SOURCE} ${PROTO_MSG_HEADER})

target_link_libraries(proto_messages ${Protobuf_LIBRARIES})

message(STATUS "[-]: ${PROTO_MSG_SOURCE}")
message(STATUS "[-]: ${PROTO_MSG_HEADER}")