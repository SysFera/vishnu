# Find Jansson Json library

# This module defines:
# LIBJANSSON_FOUND          - whether library was found
# LIBJANSSON_INCLUDE_DIR    - include path
# LIBJANSSON_LIB      - libraries to link against

find_path(LIBJANSSON_INCLUDE_DIR
  NAMES jansson.h
  PATHS
  ${LIBJANSSON_DIR}/include
  $ENV{LIBJANSSON_DIR}/include
  /usr/local/include
  /opt/local/include
  ${LIBJANSSON_INCLUDE_DIR}
)


find_library(LIBJANSSON_LIB
  NAMES jansson
  PATHS
  ${LIBJANSSON_DIR}/lib
  $ENV{LIBJANSSON_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBJANSSON_LIBRARY_DIR}
)


if (LIBJANSSON_INCLUDE_DIR AND  LIBJANSSON_LIB)
  set(LIBJANSSON_FOUND "Yes")
  mark_as_advanced(LIBJANSSON_INCLUDE_DIR)
  mark_as_advanced(LIBJANSSON_LIB)
endif(LIBJANSSON_INCLUDE_DIR AND  LIBJANSSON_LIB)
