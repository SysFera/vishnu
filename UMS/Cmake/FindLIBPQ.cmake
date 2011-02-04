#
# Try to find the TORQUE installation
#

find_library(LIBPQ_LIB
  pq 
  paths
	${LIBPQ}/lib
        $ENV{LIBPQ}/lib
	/usr/lib
	/usr/local/lib
)

if (LIBPQ_LIB)
  set(LIBPQ_FOUND "Yes")
  mark_as_advanced(LIBPQ_DIR)
  mark_as_advanced(LIBPQ_LIB)
endif(LIBPQ_LIB)


