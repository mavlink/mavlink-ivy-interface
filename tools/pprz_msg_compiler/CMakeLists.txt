# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 


project(pprz_msg_parser)
cmake_minimum_required (VERSION 2.6) 

file(GLOB pprz_msg_srcs "*.cpp")
file(GLOB pprz_msg_incl "*.h")
file(GLOB_RECURSE pprz_msg_tpls "*.tpl")

source_group("Src" FILES ${pprz_msg_srcs})
source_group("Includes" FILES ${pprz_msg_incl})
source_group("Templates" FILES ${pprz_msg_tpls})

STRING(REGEX REPLACE ".tpl" ".h" pprz_msg_tpl_h "${pprz_msg_tpls}")

# Compression of the manuals
FOREACH(tpl ${pprz_msg_tpls})
  STRING(REGEX REPLACE ".tpl" ".h" tpl_h "${tpl}")
  ADD_CUSTOM_COMMAND(OUTPUT ${tpl_h}
                     COMMAND templater ${tpl} ${tpl_h}
                     DEPENDS ${tpl}
                     COMMENT "Building TPL template ${tpl_h}")
ENDFOREACH(tpl)


include_directories (${SMARTUAV_SOURCE_DIR}/tinyxml) 

add_executable (pprz_msg_parser ${pprz_msg_srcs} ${pprz_msg_incl} ${pprz_msg_tpls} ${pprz_msg_tpl_h} )

target_link_libraries (pprz_msg_parser tinyxml)

add_dependencies(pprz_msg_parser templater) 
