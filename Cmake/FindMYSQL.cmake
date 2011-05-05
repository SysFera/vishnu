#
# Try to find the MYSQL installation
#

SET( MYSQL_FOUND_STRING "Whether a MYSQL installation was found." )

find_path(MYSQL_INCLUDE_DIR
  mysql.h
  paths
  ${MYSQL_DIR}/include
  $ENV{MYSQL_DIR}/include
  /usr/include
  /usr/local/include
  /opt/local/include
)

find_library(MYSQL_LIB
  mysql
  paths
  ${MYSQL_DIR}
  ${MYSQL_DIR}/lib
  $ENV{MYSQL_DIR}/lib
  /usr/lib
  /usr/local/lib
)

if (MYSQL_INCLUDE_DIR)
  set(MYSQL_FOUND TRUE CACHE BOOL ${MYSQL_FOUND_STRING} FORCE)
  mark_as_advanced(MYSQL_DIR)
  mark_as_advanced(MYSQL_LIBS)
else (MYSQL_INCLUDE_DIR)
  set(MYSQL_FOUND FALSE CACHE BOOL ${MYSQL_FOUND_STRING} FORCE)
endif(MYSQL_INCLUDE_DIR)

