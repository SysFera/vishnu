#    Find Sigar library

# This module defines:
# SIGAR_FOUND		- whether library was found
# SIGAR_INCLUDE_DIR	- include path
# SIGAR_LIBRARIES	- libraries to link against

# get some hints from pkg-config
if(NOT WIN32)
  include(FindPkgConfig)
  # run quietly since we only retrieve some hints from pkg-config
  pkg_check_modules(SIGAR QUIET sigar)
  # returns SIGAR_INCLUDE_DIRS and SIGAR_LIBRARY_DIRS
endif()

find_path(SIGAR_INCLUDE_DIR
  NAMES sigar.h
  PATHS ${SIGAR_INSTALL_DIR}/include
  ${SIGAR_INCLUDE_DIR}
)

find_library(SIGAR_LIBRARIES
  NAMES sigar
  PATHS ${SIGAR_INSTALL_DIR}/lib
  ${SIGAR_LIBRARY_DIRS}
)

# defines our variables
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SIGAR DEFAULT_MSG
  SIGAR_LIBRARIES SIGAR_INCLUDE_DIR)
