#
#
# Main CMakeLists.txt
# Questions: C. De Wagter
#
#

#######################################################################
# basic setup
#######################################################################

PROJECT(mavlink-ivy-interface)

cmake_minimum_required(VERSION 2.6)

set( MAVLINK_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../mavlink/include" )
set( MAVLINK_TYPES_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../mavlink/include/v1.0" )

set(EXECUTABLE_OUTPUT_PATH ${mavlink-ivy-interface_SOURCE_DIR}/bin)


#######################################################################
# directory structure
#######################################################################

#######################################################################
# platform dependent information
#######################################################################

if(WIN32)
   if(MSVC)
      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
else()
   add_definitions(-Wno-write-strings)
endif(WIN32)

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" )

FIND_PACKAGE(MAVLINK   REQUIRED)
FIND_PACKAGE(IVY       REQUIRED)
FIND_PACKAGE(GLIB2     REQUIRED)
FIND_PACKAGE(GTHREAD2  REQUIRED)

add_subdirectory (tools/templater)
add_subdirectory (tools/tinyxml)
add_subdirectory (tools/math)
add_subdirectory (tools/pprz_msg_compiler)

add_subdirectory (paparazzi)

add_subdirectory (interface)





