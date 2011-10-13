# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 


project(mavlink-example)
cmake_minimum_required(VERSION 2.6)

include_directories("${PROJECT_SOURCE_DIR}/../include/common")

add_executable (mavlink-example "mavlink_udp.c" )

