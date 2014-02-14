# Find Jansson Json library

# This module defines:
# ENABLE_MUNGE          - whether library was found
# MUNGE_INCLUDE_DIR    - include path
# MUNGE_LIB      - libraries to link against

find_path(MUNGE_INCLUDE_DIR
  NAMES munge.h
  PATHS
  ${MUNGE_DIR}/include
  $ENV{MUNGE_DIR}/include
  /usr/local/include
  /opt/local/include
  ${MUNGE_INCLUDE_DIR}
)


find_library(MUNGE_LIB
  NAMES munge
  PATHS
  ${MUNGE_DIR}/lib
  $ENV{MUNGE_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${MUNGE_LIBRARY_DIR}
)


if (MUNGE_INCLUDE_DIR AND MUNGE_LIB)
  set(ENABLE_MUNGE ON)
  mark_as_advanced(MUNGE_INCLUDE_DIR)
  mark_as_advanced(MUNGE_LIB)
endif(MUNGE_INCLUDE_DIR AND  MUNGE_LIB)
