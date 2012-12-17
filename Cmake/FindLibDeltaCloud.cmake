# Find Deltacloud library

# This module defines:
# LIBDELTACLOUD_FOUND          - whether library was found
# LIBDELTACLOUD_INCLUDE_DIR    - include path
# LIBDELTACLOUD_LIBRARIES      - libraries to link against



find_path(LIBDELTACLOUD_INCLUDE_DIR
  NAMES libdeltacloud/libdeltacloud.h
  PATHS
  ${LIBDELTACLOUD_DIR}/include
  $ENV{LIBDELTACLOUD_DIR}/include
  /usr/local/include
  /opt/local/include
  ${LIBDELTACLOUD_INCLUDE_DIRS}
)


find_library(LIBDELTACLOUD_LIB
  NAMES deltacloud
  PATHS
  ${LIBDELTACLOUD_DIR}/lib
  $ENV{LIBDELTACLOUD_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBDELTACLOUD_LIBRARY_DIRS}
)


if (LIBDELTACLOUD_INCLUDE_DIR AND  LIBDELTACLOUD_LIB)
  set(LIBDELTACLOUD_FOUND "Yes")
  mark_as_advanced(LIBDELTACLOUD_INCLUDE_DIR)
  mark_as_advanced(LIBDELTACLOUD_LIB)
endif(LIBDELTACLOUD_INCLUDE_DIR AND  LIBDELTACLOUD_LIB)
