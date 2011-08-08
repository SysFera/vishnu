#
# Try to find the SLURM installation
#

find_path(SLURM_INCLUDE_DIR
  slurm/slurm.h
  paths
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
  /opt/local/lib
)

if (SLURM_INCLUDE_DIR AND  SLURM_LIB)
  set(SLURM_FOUND "Yes")
  mark_as_advanced(SLURM_INCLUDE_DIR)
  mark_as_advanced(SLURM_LIB)
endif(SLURM_INCLUDE_DIR AND SLURM_LIB)

