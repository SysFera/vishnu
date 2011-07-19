cmake_minimum_required(VERSION 2.6)

add_subdirectory(utils-slurm)

#option(BUILD_SHARED_LIBS "disable shared libraries usage" ON)

find_library(SLURM_LIB
  slurm 
  paths
  ${SLURM_DIR}/lib
        $ENV{SLURM_DIR}/lib
  /usr/lib
  /usr/local/lib
)


include_directories(
  /usr/local/include/slurm/
  /home/traore/VishnuProject/vishnu/TMS/test-slurm/Mytest/utils-slurm
)

add_executable(test-slurm-submit
  test-slurm-submit
)

target_link_libraries(test-slurm-submit
 ${SLURM_DIR}
 utils-slurm
)
