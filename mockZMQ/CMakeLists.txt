include_directories(
  ${ZMQ_INCLUDE_DIR}
  ZMQHelper
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
)


add_library(zmq_helper
  DIET_client.cpp
  mockDagda.cpp
  Message.cpp
  Annuary.cpp
  Server.cpp
  Handler.cpp
  TreatmentData.cpp
  BasicHandler.cpp
  HandlerFactory.cpp
  ZMQHelper/mdclient.cpp)

target_link_libraries (zmq_helper
  ${ZMQ_LIBRARIES} ${Boost_LIBRARIES})

add_executable(namer
  namer.cpp
  )

target_link_libraries(namer
  ${ZMQ_LIBRARIES} 
  ${Boost_LIBRARIES}
  zmq_helper
  vishnu-core
)

# --------------------------------------------------------------------------
#add_subdirectory(ZMQHelper)
# --------------------------------------------------------------------------


## eof - CMakeLists.txt
