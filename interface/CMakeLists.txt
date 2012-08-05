
project(mavlink-ivy-interface)
cmake_minimum_required (VERSION 2.6) 

file(GLOB interface_srcs "*.cpp")
file(GLOB interface_incl "*.h")

source_group("Src" FILES ${interface_srcs})
source_group("Includes" FILES ${interface_incl})


set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake" )

FIND_PACKAGE( GLIB2 REQUIRED )
FIND_PACKAGE( GTHREAD2 REQUIRED )
FIND_PACKAGE( IVY REQUIRED )

add_definitions(-Wall )

INCLUDE_DIRECTORIES(
  ${GLIB2_MAIN_INCLUDE_DIR}
  ${GLIB2_INTERNAL_INCLUDE_DIR}
  ${GTHREAD2_MAIN_INCLUDE_DIR}
  ${IVY_INCLUDE_DIR}
  ${GLIBIVY_INCLUDE_DIR}
  ${GLIB2_MAIN_INCLUDE_DIR}
  ${GLIB2_INTERNAL_INCLUDE_DIR}
  ${GTHREAD2_MAIN_INCLUDE_DIR}
)

include_directories("${PROJECT_SOURCE_DIR}/../mavlink/include/v1.0/common")
include_directories("${uav_math_SOURCE_DIR}")

add_executable (mavlink-ivy-interface ${interface_srcs} ${interface_incl} )

target_link_libraries(mavlink-ivy-interface ${GLIB2_LIBRARY} ${GTHREAD2_LIBRARY} ${GLIBIVY_LIBRARY_OPTIMIZED} )
target_link_libraries(mavlink-ivy-interface uav_math)
#target_link_libraries(mavlink-ivy-interface lpcre)


add_dependencies(mavlink-ivy-interface interface_parser uav_math) 
