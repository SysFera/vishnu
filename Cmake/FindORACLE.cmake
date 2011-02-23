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

find_library(ORACLE_OCCI_LIB
  occi
  paths
  ${ORACLE_DIR}
  ${ORACLE_DIR}/lib
  $ENV{ORACLE_DIR}/lib
  /usr/lib
  /usr/local/lib
)

find_library(ORACLE_CLNTSH_LIB
  clntsh
  paths
  ${ORACLE_DIR}
  ${ORACLE_DIR}/lib
  $ENV{ORACLE_DIR}/lib
  /usr/lib
  /usr/local/lib
)

if (ORACLE_INCLUDE_DIR)
  set(ORACLE_FOUND TRUE CACHE BOOL ${ORACLE_FOUND_STRING} FORCE)
  mark_as_advanced(ORACLE_DIR)
  mark_as_advanced(ORACLE_LIBS)
else (ORACLE_INCLUDE_DIR)
  set(ORACLE_FOUND FALSE CACHE BOOL ${ORACLE_FOUND_STRING} FORCE)
endif(ORACLE_INCLUDE_DIR)

