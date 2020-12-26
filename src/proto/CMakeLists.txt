# https://cmake.org/cmake/help/latest/module/FindProtobuf.html

protobuf_generate_cpp(PROTO_MSG_SOURCE PROTO_MSG_HEADER Direction.proto Goal.proto Turn.proto MsgId.proto Message.proto)

add_library(proto_messages ${PROTO_MSG_SOURCE} ${PROTO_MSG_HEADER})
target_link_libraries(proto_messages ${Protobuf_LIBRARIES})
