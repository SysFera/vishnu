#
# Try to find the LOADLEVELER installation
#

find_path(LSF_INCLUDE_DIR
 lsf/lsbatch.h
  paths
	${LSF_DIR}/include
	$ENV{LSF_DIR}/include
	/usr/include
	/usr/local/include
	/opt/local/include
)

find_library(LSBATCH_LIB
  libbat
  paths
	${LSF_DIR}/lib
  $ENV{LSF_DIR}/lib
	/usr/lib
	/usr/local/lib
  /opt/local/lib
)


find_library(LSF_LIB
  liblsf
  paths
	${LSF_DIR}/lib
  $ENV{LSF_DIR}/lib
	/usr/lib
	/usr/local/lib
  /opt/local/lib
)

if (LSF_INCLUDE_DIR AND  LSF_LIB)
  set(LSF_FOUND "Yes")
  mark_as_advanced(LSF_INCLUDE_DIR)
  mark_as_advanced(LSBATCH_LIB)
  mark_as_advanced(LSF_LIB)
endif(LSF_INCLUDE_DIR AND LSF_LIB)


