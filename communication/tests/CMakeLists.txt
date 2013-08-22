
include_directories(${CONFIG_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${VISHNU_SOURCE_DIR}/communication
  ${VISHNU_SOURCE_DIR}/communication/dispatcher
  ${VISHNU_SOURCE_DIR}/communication/tests
  ${VISHNU_SOURCE_DIR}/TMS/src/sed
  ${VISHNU_SOURCE_DIR}/UMS/src/sed
  ${VISHNU_SOURCE_DIR}/FMS/src/sed
  ${VISHNU_SOURCE_DIR}/IMS/src/sed
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
  ../sslhelpers.cpp
  )

add_library(fake_zmq
  zmq.hpp
  fake.cpp
  )

target_link_libraries (test_zmq_helper
  fake_zmq
  ${Boost_LIBRARIES}
  vishnu-core
  ${OPENSSL_LIBRARIES}
  )

# register tests
unit_test(LazyPirateUnitTests test_zmq_helper zmq_helper)
unit_test(AnnuaryUnitTests test_zmq_helper zmq_helper)
unit_test(ZMQServerUnitTests test_zmq_helper zmq_helper)
unit_test(DIET_clientUnitTests test_zmq_helper zmq_helper)
