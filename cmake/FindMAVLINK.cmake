INCLUDE(FindPackageHandleStandardArgs)

SET(MAVLINK_IncludeSearchPaths
  /usr/include
  /usr/local/include
  /opt/local/include
  mavlink/include/common
  ../mavlink/include
  ../mavlink/include/common
)

FIND_PATH(MAVLINK_INCLUDE_DIR
  NAMES mavlink.h
  PATHS ${MAVLINK_IncludeSearchPaths}
)

FIND_PATH(MAVLINK_TYPES_INCLUDE_DIR
  NAMES mavlink_types.h
  PATHS ${MAVLINK_IncludeSearchPaths}
)

# Handle the REQUIRED argument and set the <UPPERCASED_NAME>_FOUND variable
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MAVLINK "Could NOT find MAVLink protocol (mavlink.h)"
  MAVLINK_INCLUDE_DIR
  MAVLINK_TYPES_INCLUDE_DIR
)

MARK_AS_ADVANCED(
  MAVLINK_INCLUDE_DIR
  MAVLINK_TYPES_INCLUDE_DIR
)
