#
# Try to find the POSTGRESQL installation
#

find_path(POSTGRESQL_INCLUDE_DIR
 postgresql/libpq-fe.h
  paths
	${POSTGRESQL}/include
	$ENV{POSTGRESQL}/include
	/usr/include
	/usr/local/include
	/opt/local/include
)

if (POSTGRESQL_INCLUDE_DIR)
  set(POSTGRESQL_FOUND "Yes")
  mark_as_advanced(POSTGRESQL_DIR)
endif(POSTGRESQL_INCLUDE_DIR)


