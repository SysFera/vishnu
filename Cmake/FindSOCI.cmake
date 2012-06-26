#
# Try to find the SOCI installation
#

SET( SOCI_FOUND_STRING "Whether a SOCI installation was found." )

set(SOCI_DBMS_LIB_NAME)
set(SOCI_DBMS_DIR)
set(SOCI_INCLUDE_NAMES soci.h soci-backend.h)
if(ENABLE_MYSQL)
  set(SOCI_DBMS_DIR mysql)
  set(SOCI_INCLUDE_NAMES ${SOCI_INCLUDE_NAMES} ${SOCI_DMBS_DIR}/soci-mysql.h)
endif(ENABLE_MYSQL)
if(ENABLE_POSTGRESQL)
    set(SOCI_DBMS_DIR postgresql)
    set(SOCI_INCLUDE_NAMES ${SOCI_INCLUDE_NAMES} ${SOCI_DMBS_DIR}/soci-postgresql.h)
endif(ENABLE_POSTGRESQL)


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


if (SOCI_INCLUDE_DIR)
  set(SOCI_FOUND TRUE CACHE BOOL ${SOCI_FOUND_STRING} FORCE)
  mark_as_advanced(SOCI_DIR)
  mark_as_advanced(SOCI_LIBS)
else (SOCI_INCLUDE_DIR)
  set(SOCI_FOUND FALSE CACHE BOOL ${SOCI_FOUND_STRING} FORCE)
endif(SOCI_INCLUDE_DIR)

