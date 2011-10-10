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

add_subdirectory (tools/templater)
add_subdirectory (tools/tinyxml)
add_subdirectory (tools/pprz_msg_compiler)

add_subdirectory (paparazzi)



