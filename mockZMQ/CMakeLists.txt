include_directories(
  ${ZMQ_INCLUDE_DIR}
  ZMQHelper
  ${UTILVISHNU_SOURCE_DIR}
)


add_library(zmq_helper
  DIET_client.cpp
  Message.cpp
  Annuary.cpp
  Server.cpp
  Handler.cpp
  TreatmentData.hpp
  BasicHandler.cpp
  HandlerFactory.cpp
  ZMQHelper/mdclient.cpp)

target_link_libraries (zmq_helper
  ${ZMQ_LIBRARIES} ${Boost_LIBRARIES})


# --------------------------------------------------------------------------
#add_subdirectory(ZMQHelper)
# --------------------------------------------------------------------------


## eof - CMakeLists.txt
