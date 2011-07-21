cmake_minimum_required(VERSION 2.6)

#option(BUILD_SHARED_LIBS "disable shared libraries usage" ON)

link_directories(
  /usr/local/lib
)

find_library(SLURM_LIB 
  slurm 
  paths
  ${SLURM_DIR}/lib
  $ENV{SLURM_DIR}/lib
  /usr/lib
  /usr/local/lib
)
MESSAGE("BEFORE OLD_CMAKE_FIND_LIBRARY_SUFFIXES:"${CMAKE_FIND_LIBRARY_SUFFIXES})
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

MESSAGE("NEW_CMAKE_FIND_LIBRARY_SUFFIXES:"${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES ${OLD_CMAKE_FIND_LIBRARY_SUFFIXES})
MESSAGE("AFTER OLD_CMAKE_FIND_LIBRARY_SUFFIXES:"${CMAKE_FIND_LIBRARY_SUFFIXES})

include_directories(
  /usr/local/include/slurm/
  /home/traore/VishnuProject/vishnu/TMS/test-slurm/Mytest/utils-slurm
)

add_library(utils-slurm STATIC 
  opt.c
  mult_cluster.c
  slurm-submit.c 
)

message("slurm lib dir: "${SLURM_LIB})
message("slurm static lib dir: "${SLURM_STATIC_LIB})
target_link_libraries(utils-slurm
  ${SLURM_LIB}
  ${SLURM_STATIC_LIB}
  pthread
  dl
  c
)
