cmake_minimum_required(VERSION 2.8.12)
project(texas_code)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.cc)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${headers} ${sources})
# add_subdirectory(proto)
# add_dependencies(${PROJECT_NAME} pb_generator)

include_directories(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/include)

find_package(Protobuf REQUIRED)
message(STATUS ${PROTOBUF_INCLUDE_DIRS})
message(STATUS ${PROTOBUF_LIBRARIES})
include_directories(${PROJECT_NAME} ${PROTOBUF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})

find_package(ZeroMQ)
message(STATUS ${ZeroMQ_INCLUDE_DIRS})
message(STATUS ${ZeroMQ_LIBRARIES})
include_directories(${PROJECT_NAME} ${ZeroMQ_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ZeroMQ_LIBRARIES})

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.60.0 COMPONENTS system thread REQUIRED)
message(STATUS ${Boost_INCLUDE_DIRS})
message(STATUS ${Boost_LIBRARIES})
include_directories(${PROJECT_NAME} ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
