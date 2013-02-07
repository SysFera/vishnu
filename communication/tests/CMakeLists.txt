
include_directories(${CONFIG_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${VISHNU_SOURCE_DIR}/communication
  ${VISHNU_SOURCE_DIR}/communication/tests
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
  fake_zmq
  ${Boost_LIBRARIES}
  vishnu-core
  )

# register tests
unit_test(LazyPirateUnitTests test_zmq_helper)
unit_test(AnnuaryUnitTests test_zmq_helper)
unit_test(ZMQServerUnitTests test_zmq_helper)
unit_test(DIET_clientUnitTests test_zmq_helper)
