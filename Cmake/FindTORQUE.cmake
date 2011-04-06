#
# Try to find the TORQUE installation
#

find_path(TORQUE_INCLUDE_DIR
 pbs_ifl.h
  paths
	${TORQUE_DIR}/include
	$ENV{TORQUE_DIR}/include
	/usr/include
	/usr/local/include
	/opt/local/include
)

find_path(TORQUE_INCLUDE_DIR
 pbs_error.h
  paths
        ${TORQUE_DIR}/include
        $ENV{TORQUE_DIR}/include
        /usr/include
        /usr/local/include
        /opt/local/include
)


find_library(TORQUE_LIB
  torque 
  paths
	${TORQUE_DIR}/lib
        $ENV{TORQUE_DIR}/lib
	/usr/lib
	/usr/local/lib
)


if (TORQUE_INCLUDE_DIR AND  TORQUE_LIB)
  set(TORQUE_FOUND "Yes")
  mark_as_advanced(TORQUE_DIR)
  mark_as_advanced(TORQUE_LIB)
endif(TORQUE_INCLUDE_DIR AND TORQUE_LIB)


