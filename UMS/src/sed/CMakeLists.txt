
include_directories(
  /home/capo-chichi/Sysfera/DIET_VISHNU/include
  /usr/include/postgresql
)

add_executable(
 main main.cc ServerUMS.cc ServerUMS.hh
)

target_link_libraries(main 
  /usr/local/lib/libDIET_SeD.so
)






