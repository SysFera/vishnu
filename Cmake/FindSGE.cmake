#
# Try to find the SGE installation
#

find_path(SGE_INCLUDE_DIR
 drmaa.h
  paths
        ${SGE_DIR}/include
        $ENV{SGE_DIR}/include
        /usr/include
        /usr/local/include
        /opt/local/include
)

find_library(SGE_LIB
  drmaa 
  paths
        ${SGE_DIR}/lib
	$ENV{SGE_DIR}/lib/linux-x86
        /usr/lib
        /usr/local/lib
	/opt/local/lib
)

find_path(SGE_ROOT
  inst_sge
  paths
      $ENV{SGE_ROOT}
)
if (SGE_INCLUDE_DIR AND  SGE_LIB AND SGE_ROOT)
  set(SGE_FOUND "Yes")
  mark_as_advanced(SGE_ROOT)
  mark_as_advanced(SGE_INCLUDE_DIR)
  mark_as_advanced(SGE_LIB)
endif(SGE_INCLUDE_DIR AND  SGE_LIB AND SGE_ROOT)