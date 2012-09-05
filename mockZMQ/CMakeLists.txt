if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

include_directories(
  ${ZMQ_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_SOURCE_DIR}/mockZMQ/ZMQHelper
)


add_library(zmq_helper
  DIET_client.cpp
  Annuary.cpp
  Server.cpp
  SeD.cpp
  utils.cpp
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
