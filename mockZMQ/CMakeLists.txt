
set( ZMQ_LIB ""
    CACHE PATH "Path to zmqlib" )
set( ZMQ_INCLUDE_DIR ""
    CACHE PATH "Path to zmq include dir" )

include_directories(
  ${ZMQ_INCLUDE_DIR}
  ZMQHelper
  )


add_library(zmq_helper
  DIET_client.cpp
  ZMQHelper/mdclient.cpp
)

target_link_libraries (zmq_helper
  ${ZMQ_LIB}
)


# --------------------------------------------------------------------------
#add_subdirectory(ZMQHelper)
# --------------------------------------------------------------------------


## eof - CMakeLists.txt
