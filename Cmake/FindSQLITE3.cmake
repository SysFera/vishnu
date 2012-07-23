#
# Try to find the SQLITE3 installation
#

SET( SQLITE3_FOUND_STRING "Whether a SQLITE3 installation was found." )

find_path(SQLITE3_INCLUDE_DIR
  sqlite3.h
  paths
  ${SQLITE3_DIR}/include
  $ENV{SQLITE3_DIR}/include
  /usr/include
  /usr/include/sqlite3
  /usr/local/include
  /opt/local/include
)

find_library(SQLITE3_LIB
  NAMES sqlite3
  PATHS
  ${SQLITE3_DIR}
  ${SQLITE3_DIR}/lib
  $ENV{SQLITE3_DIR}/lib
  /usr/lib
  /usr/lib64
  /usr/local/lib
  /usr/local/lib64
  PATH_SUFFIXES sqlite3
)

if (SQLITE3_INCLUDE_DIR)
  set(SQLITE3_FOUND TRUE CACHE BOOL ${SQLITE3_FOUND_STRING} FORCE)
  mark_as_advanced(SQLITE3_DIR)
  mark_as_advanced(SQLITE3_LIBS)
else (SQLITE3_INCLUDE_DIR)
  set(SQLITE3_FOUND FALSE CACHE BOOL ${SQLITE3_FOUND_STRING} FORCE)
endif(SQLITE3_INCLUDE_DIR)

