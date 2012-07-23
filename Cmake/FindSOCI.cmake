#
# Try to find the SOCI installation
#

SET( SOCI_FOUND_STRING "Whether a SOCI installation was found." )

set(SOCI_INCLUDE_NAMES soci.h soci-backend.h)
if(ENABLE_MYSQL)
  set(SOCI_INCLUDE_NAMES ${SOCI_INCLUDE_NAMES} mysql/soci-mysql.h)
endif(ENABLE_MYSQL)
if(ENABLE_POSTGRESQL)
    set(SOCI_INCLUDE_NAMES ${SOCI_INCLUDE_NAMES} postgresql/soci-postgresql.h)
endif(ENABLE_POSTGRESQL)
if(ENABLE_ORACLE)
    set(SOCI_INCLUDE_NAMES ${SOCI_INCLUDE_NAMES} oracle/soci-oracle.h)
endif(ENABLE_ORACLE)
if(ENABLE_SQLITE3)
    set(SOCI_INCLUDE_NAMES ${SOCI_INCLUDE_NAMES} sqlite3/soci-sqlite3.h)
endif(ENABLE_SQLITE3)


find_path(SOCI_INCLUDE_DIR
  NAMES ${SOCI_INCLUDE_NAMES}
  PATHS
  ${SOCI_DIR}/include
  $ENV{SOCI_DIR}/include
  /usr/local/include/soci
  /usr/include
  /usr/include/soci
)


find_library(SOCI_LIB
  NAMES soci_core 
  PATHS
  ${SOCI_DIR}
  ${SOCI_DIR}/lib
  $ENV{SOCI_DIR}/lib
  /usr/local/lib64
  /usr/lib
  /usr/lib64
  /usr/local/lib
  PATH_SUFFIXES soci/${SOCI_DMBS_DIR}
)
set (SOCI_LIBS ${SOCI_LIB})


if(ENABLE_MYSQL)
	find_library(SOCI_MYSQL_LIB
	  NAMES soci_mysql 
	  PATHS
	  ${SOCI_DIR}
	  ${SOCI_DIR}/lib
	  $ENV{SOCI_DIR}/lib
	  /usr/local/lib64
	  /usr/lib
	  /usr/lib64
	  /usr/local/lib
	  PATH_SUFFIXES soci
	)
	set(SOCI_LIBS ${SOCI_LIBS} ${SOCI_MYSQL_LIB})
endif(ENABLE_MYSQL)

if(ENABLE_POSTGRESQL)
	find_library(SOCI_POSTGRESQL_LIB
	  NAMES soci_postgresql 
	  PATHS
	  ${SOCI_DIR}
	  ${SOCI_DIR}/lib
	  $ENV{SOCI_DIR}/lib
	  /usr/local/lib64
	  /usr/lib/
	  /usr/lib64/
	  /usr/local/lib
	  PATH_SUFFIXES soci
	)
	set(SOCI_LIBS ${SOCI_LIBS} ${SOCI_POSTGRESQL_LIB})
endif(ENABLE_POSTGRESQL)

if(ENABLE_ORACLE)
	find_library(SOCI_ORACLE_LIB
	  NAMES soci_oracle 
	  PATHS
	  ${SOCI_DIR}
	  ${SOCI_DIR}/lib
	  $ENV{SOCI_DIR}/lib
	  /usr/local/lib64
	  /usr/lib
	  /usr/lib64
	  /usr/local/lib
	  PATH_SUFFIXES soci
	)
	set(SOCI_LIBS ${SOCI_LIBS} ${SOCI_ORACLE_LIB})
endif(ENABLE_ORACLE)

if(ENABLE_SQLITE3)
	find_library(SOCI_SQLITE3_LIB
	  NAMES soci_sqlite3
	  PATHS
	  ${SOCI_DIR}
	  ${SOCI_DIR}/lib
	  $ENV{SOCI_DIR}/lib
	  /usr/local/lib64
	  /usr/lib
	  /usr/lib64
	  /usr/local/lib
	  PATH_SUFFIXES soci
	)
	set(SOCI_LIBS ${SOCI_LIBS} ${SOCI_SQLITE3_LIB})
endif(ENABLE_SQLITE3)

if (SOCI_INCLUDE_DIR)
  set(SOCI_FOUND TRUE CACHE BOOL ${SOCI_FOUND_STRING} FORCE)
  mark_as_advanced(SOCI_DIR)
  mark_as_advanced(SOCI_LIBS)
else (SOCI_INCLUDE_DIR)
  set(SOCI_FOUND FALSE CACHE BOOL ${SOCI_FOUND_STRING} FORCE)
endif(SOCI_INCLUDE_DIR)

