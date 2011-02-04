#
# Try to find the DIET installation
#

find_path(DIET_INCLUDE_DIR
  DIET_data.h
  paths
	${DIET_DIR}/include
	$ENV{DIET_DIR}/include
	/usr/include
	/usr/local/include
	/opt/local/include
)

find_library(DIET_CLIENT_LIB
  DIET_client
  paths
	${DIET_DIR}/lib
        $ENV{DIET_DIR}/lib
	/usr/lib
	/usr/local/lib
)

find_library(DIET_SED_LIB
  DIET_SeD
  paths
        ${DIET_DIR}/lib
        $ENV{DIET_DIR}/lib
        /usr/lib
        /usr/local/lib
)

if (DIET_INCLUDE_DIR AND DIET_CLIENT_LIB AND DIET_SED_LIB)
  set(DIET_FOUND "Yes")
  mark_as_advanced(DIET_DIR)
  mark_as_advanced(DIET_CLIENT_LIB)
  mark_as_advanced(DIET_SED_LIB)
endif(DIET_INCLUDE_DIR AND DIET_CLIENT_LIB AND DIET_SED_LIB)
