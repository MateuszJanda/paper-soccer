# https://cmake.org/cmake/help/latest/module/FindProtobuf.html

set(PROTO_NAME paper_soccer_proto)

protobuf_generate_cpp(PROTO_MSG_SOURCE PROTO_MSG_HEADER
    Direction.proto
    Goal.proto
    Turn.proto
    MsgId.proto
    Message.proto
)

add_library(${PROTO_NAME} ${PROTO_MSG_SOURCE} ${PROTO_MSG_HEADER})
target_link_libraries(${PROTO_NAME} ${Protobuf_LIBRARIES})
