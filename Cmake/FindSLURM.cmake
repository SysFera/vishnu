#
# Try to find the SLURM installation
#

find_path(SLURM_INCLUDE_DIR
  slurm.h
  paths
	${SLURM_DIR}/include/slurm
	$ENV{SLURM_DIR}/include/slurm
	/usr/include/slurm
	/usr/local/include/slurm
	/opt/local/include/slurm
  ${SLURM_DIR}/include
  $ENV{SLURM_DIR}/include
  /usr/include
  /usr/local/include
  /opt/local/include
)


find_library(SLURM_LIB
  slurm 
  paths
	${SLURM_DIR}/lib
        $ENV{SLURM_DIR}/lib
	/usr/lib
	/usr/local/lib
)

set(OLD_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so")

find_library(SLURM_STATIC_LIB 
  slurm 
  paths
  ${SLURM_DIR}/lib
  $ENV{SLURM_DIR}/lib
  /usr/lib
  /usr/local/lib
)

set(CMAKE_FIND_LIBRARY_SUFFIXES ${OLD_CMAKE_FIND_LIBRARY_SUFFIXES})

if (SLURM_INCLUDE_DIR AND  SLURM_LIB AND SLURM_STATIC_LIB)
  set(SLURM_FOUND "Yes")
  mark_as_advanced(SLURM_INCLUDE_DIR)
  mark_as_advanced(SLURM_LIB)
  mark_as_advanced(SLURM_STATIC_LIB)
endif(SLURM_INCLUDE_DIR AND SLURM_LIB AND SLURM_STATIC_LIB)

