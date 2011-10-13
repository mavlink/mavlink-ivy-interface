
project(pprz_reader)
cmake_minimum_required (VERSION 2.6) 

file(GLOB pprz_msg_srcs "*.cpp")
file(GLOB pprz_msg_incl "*.h")
file(GLOB_RECURSE pprz_msg_xmls "*.xml")

source_group("Src" FILES ${pprz_msg_srcs})
source_group("Includes" FILES ${pprz_msg_incl})
source_group("Xml" FILES ${pprz_msg_xmls})

STRING(REGEX REPLACE ".xml" ".h" pprz_msg_xml_h "${pprz_msg_xmls}")

FOREACH(xml ${pprz_msg_xmls})
  STRING(REGEX REPLACE ".xml" ".h" xml_h "${xml}")
  ADD_CUSTOM_COMMAND(OUTPUT ${xml_h}
                     COMMAND pprz_msg_parser ${xml} ${xml_h}
                     DEPENDS ${xml}
                     COMMENT "Building Paparazzi Messages ${xml_h}")
ENDFOREACH(xml)

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/.." )

FIND_PACKAGE( GLIB2 REQUIRED )

add_definitions(-Wall )

#include_directories("${GLIB2_INCLUDE_DIRS}")

add_definitions(  `pkg-config glib-2.0 --cflags`)

#SET(CMAKE_EXE_LINKER_FLAGS " `pkg-config glib-2.0 --libs` -lglibivy -lpcre ")

SET(CMAKE_EXE_LINKER_FLAGS "-lglibivy -lpcre -glib2" )

target_link_libraries(pprz_reader ${GLIB2_LIBRARIES} )



add_executable (pprz_reader ${pprz_msg_srcs} ${pprz_msg_incl} ${pprz_msg_xmls} ${pprz_msg_xml_h} )

add_dependencies(pprz_reader pprz_msg_parser) 
