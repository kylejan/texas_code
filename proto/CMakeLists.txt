project(pb_generator)
cmake_minimum_required(VERSION 2.8.12)

add_executable(protoc IMPORTED)

if(NOT PROTOC_DIR)
    message(FATAL_ERROR "please set the directory of protoc")
endif()

set_property(TARGET protoc PROPERTY IMPORTED_LOCATION ${PROTOC_DIR}/protoc)

set(generated_proto_cpp
    "../include/texas_code/protocol/definition.pb.h"
    "../include/texas_code/protocol/definition.pb.cc"
    "../include/texas_code/protocol/message.pb.h"
    "../include/texas_code/protocol/message.pb.cc"
)

add_custom_command(
    OUTPUT ${generated_proto_cpp}
    COMMAND protoc ARGS --cpp_out=../include/texas_code/protocol ./definition.proto
    COMMAND protoc ARGS --cpp_out=../include/texas_code/protocol ./message.proto
    COMMAND python ARGS ./mesage_type_generator.py
    DEPENDS ${generated_proto_cpp}

    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(${PROJECT_NAME} ALL DEPENDS ${generated_proto_cpp})
