include_directories(
  ${ZMQ_INCLUDE_DIR}
  ZMQHelper)


add_library(zmq_helper
  DIET_client.cpp
  ZMQHelper/mdclient.cpp)

target_link_libraries (zmq_helper
  ${ZMQ_LIBRARIES} ${Boost_LIBRARIES})


# --------------------------------------------------------------------------
#add_subdirectory(ZMQHelper)
# --------------------------------------------------------------------------


## eof - CMakeLists.txt
