#
# Try to find the POSTGRESQL installation
#

SET( POSTGRESQL_FOUND_STRING "Whether a PostgreSQL installation was found." )

find_path(POSTGRESQL_INCLUDE_DIR
 libpq-fe.h
  paths
	${POSTGRESQL}/include
	$ENV{POSTGRESQL}/include
	/usr/include
	/usr/local/include
	/opt/local/include
        /usr/include/postgresql/
)

find_library(POSTGRESQL_LIB
  pq
  paths
  ${POSTGRESQL}/lib
  $ENV{POSTGRESQL}/lib
  /usr/lib
  /usr/local/lib
)

if (POSTGRESQL_INCLUDE_DIR)
  set(POSTGRESQL_FOUND TRUE CACHE BOOL ${POSTGRESQL_FOUND_STRING} FORCE)
  mark_as_advanced(POSTGRESQL_DIR)
  mark_as_advanced(POSTGRESQL_LIB)
else (POSTGRESQL_INCLUDE_DIR)
  set(POSTGRESQL_FOUND FALSE CACHE BOOL ${POSTGRESQL_FOUND_STRING} FORCE)
endif(POSTGRESQL_INCLUDE_DIR)


