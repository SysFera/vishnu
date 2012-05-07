# Find ZeroMQ library

# This module defines:
# ZMQ_FOUND          - whether library was found
# ZMQ_INCLUDE_DIR    - include path
# ZMQ_LIBRARIES      - libraries to link against

# get some hints from pkg-config
if(NOT WIN32)
  include(FindPkgConfig)
  # run quietly since we only retrieve some hints from pkg-config
  pkg_check_modules(ZMQ QUIET libzmq)
  # returns ZMQ_INCLUDE_DIRS and ZMQ_LIBRARY_DIRS
endif()

find_path(ZMQ_INCLUDE_DIR
  NAMES zmq.h zmq.hpp
  PATHS
  ${ZMQ_DIR}/include
  $ENV{ZMQ_DIR}/include
  /usr/local/include
  /opt/local/include
  ${ZMQ_INCLUDE_DIRS}
)


find_library(ZMQ_LIBRARIES
  NAMES zmq
  PATHS
  ${ZMQ_DIR}/lib
  $ENV{ZMQ_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${ZMQ_LIBRARY_DIRS}
)

# defines our variables
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZMQ DEFAULT_MSG
  ZMQ_LIBRARIES ZMQ_INCLUDE_DIR)
