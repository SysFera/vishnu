#
# Try to find the PBSPRO installation
#

find_path(PBSPRO_INCLUDE_DIR
  pbs_ifl.h
  PATHS
  ${PBSPRO_DIR}/include
  $ENV{PBSPRO_DIR}/include
  /usr/include
  /usr/local/include
  /opt/local/include
  PATH_SUFFIXES torque
)


find_library(PBSPRO_LIB
  pbs
  paths
  ${PBSPRO_DIR}/lib
  $ENV{PBSPRO_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

if (PBSPRO_INCLUDE_DIR AND  PBSPRO_LIB)
  set(PBSPRO_FOUND "Yes")
  mark_as_advanced(PBSPRO_INCLUDE_DIR)
  mark_as_advanced(PBSPRO_LIB)
endif(PBSPRO_INCLUDE_DIR AND PBSPRO_LIB)


