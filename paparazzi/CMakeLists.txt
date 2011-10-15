
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

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake" )

FIND_PACKAGE( GLIB2 REQUIRED )
FIND_PACKAGE( GTHREAD2 REQUIRED )
FIND_PACKAGE( IVY REQUIRED )

add_definitions(-Wall )

INCLUDE_DIRECTORIES(
  /usr/include
  /usr/local/include
  /opt/local/include
  ${IVY_INCLUDE_DIR}
  ${GLIBIVY_INCLUDE_DIR}
  ${GLIB2_MAIN_INCLUDE_DIR}
  ${GLIB2_INTERNAL_INCLUDE_DIR}
  ${GTHREAD2_MAIN_INCLUDE_DIR}
)

add_executable (pprz_reader ${pprz_msg_srcs} ${pprz_msg_incl} ${pprz_msg_xmls} ${pprz_msg_xml_h} )

target_link_libraries(pprz_reader ${GLIB2_LIBRARY} ${GTHREAD2_LIBRARY} ${IVY_LIBRARY_OPTIMIZED} ${GLIBIVY_LIBRARY_OPTIMIZED} )
#target_link_libraries(pprz_reader lpcre)


add_dependencies(pprz_reader pprz_msg_parser) 
