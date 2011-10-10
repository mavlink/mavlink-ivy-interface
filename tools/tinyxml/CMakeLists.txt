# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 

set(lib_srcs
  mytinyxml.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinystr.cpp
  tinyxmlparser.cpp
)

source_group("Src" FILES ${lib_srcs})

file(GLOB lib_hdrs "*.h*")
source_group("Include" FILES ${lib_hdrs})

add_library (tinyxml ${lib_srcs} ${lib_hdrs} )

# add_library (tinyxml STATIC *.cpp *.h)
