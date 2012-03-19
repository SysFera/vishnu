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
  libdrmaa 
  paths
        ${SGE_DIR}/lib
  $ENV{SGE_DIR}/lib
        /usr/lib
        /usr/local/lib
  /opt/local/lib
)

if (SGE_INCLUDE_DIR AND  SGE_LIB)
  set(SGE_FOUND "Yes")
  mark_as_advanced(SGE_INCLUDE_DIR)
  mark_as_advanced(SGE_LIB)
endif(SGE_INCLUDE_DIR AND  SGE_LIB)