project(texas_pb)
cmake_minimum_required(VERSION 2.8.12)

add_executable(protoc IMPORTED)

if(NOT PROTOC_DIR)
    message(FATAL_ERROR "please set the directory of protoc")
endif()

set_property(TARGET protoc PROPERTY IMPORTED_LOCATION ${PROTOC_DIR}/protoc)

set(
    generated_proto_cpp
    "../include/texas_code/proto/definition.pb.h"
    "../include/texas_code/proto/definition.pb.cc"
    "../include/texas_code/proto/message.pb.h"
    "../include/texas_code/proto/message.pb.cc"
)

add_custom_command(
    OUTPUT ${generated_proto_cpp}
    COMMAND protoc ARGS --cpp_out=../include/texas_code/proto ./definition.proto
    COMMAND protoc ARGS --cpp_out=../include/texas_code/proto ./message.proto
    COMMAND python ARGS ./mesage_type_generator.py
)

add_custom_target(${PROJECT_NAME} DEPENDS ${generated_proto_cpp})
