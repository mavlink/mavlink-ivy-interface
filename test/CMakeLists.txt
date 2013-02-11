
project(test)
cmake_minimum_required (VERSION 2.6)

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake" )

add_definitions(-Wall )

include_directories("${PROJECT_SOURCE_DIR}/../mavlink/include/v1.0/common")
include_directories("${uav_math_SOURCE_DIR}")

add_executable (test ./main.cpp ../interface/udp.cpp )

target_link_libraries(test uav_math)

add_dependencies(test interface_parser uav_math)
