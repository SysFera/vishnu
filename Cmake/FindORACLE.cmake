#
# Try to find the ORACLE installation
#

SET( ORACLE_FOUND_STRING "Whether an Oracle installation was found." )

find_path(ORACLE_INCLUDE_DIR
 occi.h
  paths
  ${ORACLE_DIR}/include
  ${ORACLE_DIR}/sdk/include
  ${ORACLE_DIR}/rdbms/public
  $ENV{ORACLE_DIR}/include
  /usr/include
  /usr/local/include
  /opt/local/include
)
MESSAGE( "ORACLE_DIR: ${ORACLE_DIR}")
MESSAGE( "ORACLE_INCLUDE_DIR: ${ORACLE_INCLUDE_DIR}")
find_library(ORACLE_OCCI_LIB
  occi
  paths
  ${ORACLE_DIR}
  ${ORACLE_DIR}/lib
  $ENV{ORACLE_DIR}/lib
  /usr/lib
  /usr/local/lib
)
MESSAGE( "ORACLE_OCCI_LIB: ${ORACLE_OCCI_LIB}")
find_library(ORACLE_OCIICUS_LIB
  ociicus
  paths
  ${ORACLE_DIR}
  ${ORACLE_DIR}/lib
  $ENV{ORACLE_DIR}/lib
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

