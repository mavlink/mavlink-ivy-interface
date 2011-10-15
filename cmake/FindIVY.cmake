INCLUDE(FindPackageHandleStandardArgs)

SET(IVY_IncludeSearchPaths
  /usr/include/
  /usr/local/include/
  /opt/local/include/
)

SET(GLIBIVY_IncludeSearchPaths
  /usr/include/
  /usr/local/include/
  /opt/local/include/
)

SET(IVY_LibrarySearchPaths
  /usr/lib/
  /usr/local/lib/
  /opt/local/lib/
)

FIND_PATH(IVY_INCLUDE_DIR Ivy/ivy.h
  PATHS ${IVY_IncludeSearchPaths}
)
FIND_LIBRARY(IVY_LIBRARY_OPTIMIZED
  NAMES ivy
  PATHS ${IVY_LibrarySearchPaths}
)

FIND_PATH(GLIBIVY_INCLUDE_DIR Ivy/ivyglibloop.h
  PATHS ${GLIBIVY_IncludeSearchPaths}
)
FIND_LIBRARY(GLIBIVY_LIBRARY_OPTIMIZED
  NAMES glibivy
  PATHS ${IVY_LibrarySearchPaths}
)

# Handle the REQUIRED argument and set the <UPPERCASED_NAME>_FOUND variable
FIND_PACKAGE_HANDLE_STANDARD_ARGS(IVY "Could NOT find libivy library (IVY)"
  IVY_LIBRARY_OPTIMIZED
  IVY_INCLUDE_DIR
)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(IVY "Could NOT find libglibivy library (IVY)"
  GLIBIVY_LIBRARY_OPTIMIZED
  GLIBIVY_INCLUDE_DIR
)

# Collect optimized and debug libraries
#HANDLE_LIBRARY_TYPES(IVY)
#HANDLE_LIBRARY_TYPES(GLIBIVY)

MARK_AS_ADVANCED(
  IVY_INCLUDE_DIR
  IVY_LIBRARY_OPTIMIZED

  GLIBIVY_INCLUDE_DIR
  GLIBIVY_LIBRARY_OPTIMIZED
)
