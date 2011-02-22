#
# Try to find the ORACLE installation
#

SET( ORACLE_FOUND_STRING "Whether an Oracle installation was found." )

find_path(ORACLE_INCLUDE_DIR
 occi.h
  paths
  ${ORACLE}/include
  $ENV{ORACLE}/include
  /usr/include
  /usr/local/include
  /opt/local/include
)

find_library(ORACLE_OCCI_LIB
  occi
  paths
  ${ORACLE}/lib
  $ENV{ORACLE}/lib
  /usr/lib
  /usr/local/lib
)

find_library(ORACLE_OCIICUS_LIB
  ociicus
  paths
  ${ORACLE}/lib
  $ENV{ORACLE}/lib
  /usr/lib
  /usr/local/lib
)

if (ORACLE_INCLUDE_DIR)
  set(ORACLE_FOUND TRUE CACHE BOOL ${ORACLE_FOUND_STRING} FORCE)
  SET(ORACLE LIBS
    ${ORACLE_OCCI_LIB}
    ${ORACLE_OCIICUS_LIB}
  )
  mark_as_advanced(ORACLE_DIR)
else (ORACLE_INCLUDE_DIR)
  set(ORACLE_FOUND FALSE CACHE BOOL ${ORACLE_FOUND_STRING} FORCE)
endif(ORACLE_INCLUDE_DIR)

