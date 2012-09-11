
include_directories( ${Boost_INCLUDE_DIRS}
  ${VISHNU_SOURCE_DIR}/mockZMQ
  ${VISHNU_SOURCE_DIR}/mockZMQ/tests
  ${UTILVISHNU_SOURCE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  )

add_library(test_zmq_helper
  ../DIET_client.cpp
  ../Annuary.cpp
  ../Server.cpp
  ../SeD.cpp
  ../utils.cpp
  )

add_library(fake_zmq
  zmq.hpp
  fake.cpp
  )

target_link_libraries (test_zmq_helper
  fake_zmq ${Boost_LIBRARIES})


include(UnitTest)
unit_test_setup()
unit_test(mainTest)
