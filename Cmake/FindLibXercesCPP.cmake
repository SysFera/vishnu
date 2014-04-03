# Find Xerces-C++ library

# This module defines:
# LIBXERCESCPP_FOUND          - whether library was found
# LIBXERCESCPP_INCLUDE_DIR    - include path
# LIBXERCESCPP_LIBRARIES      - libraries to link against



find_path(LIBXERCESCPP_INCLUDE_DIR
  NAMES xercesc/util/XercesVersion.hpp
  PATHS
  ${LIBXERCESCPP_DIR}/include
  $ENV{LIBXERCESCPP_DIR}/include
  /usr/local/include
  /opt/local/include
  ${LIBXERCESCPP_INCLUDE_DIRS}
)


find_library(LIBXERCESCPP_LIB
  NAMES xerces-c
  PATHS
  ${LIBXERCESCPP_DIR}/lib
  $ENV{LIBXERCESCPP_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXERCESCPP_LIBRARY_DIRS}
)


if (LIBXERCESCPP_INCLUDE_DIR AND  LIBXERCESCPP_LIB)
  set(LIBXERCESCPP_FOUND "Yes")
  mark_as_advanced(LIBXERCESCPP_INCLUDE_DIR)
  mark_as_advanced(LIBXERCESCPP_LIB)
endif(LIBXERCESCPP_INCLUDE_DIR AND  LIBXERCESCPP_LIB)
