
include_directories(${CONFIG_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${VISHNU_SOURCE_DIR}/communication
  ${VISHNU_SOURCE_DIR}/communication/dispatcher
  ${VISHNU_SOURCE_DIR}/communication/tests
  ${VISHNU_SOURCE_DIR}/TMS/src/sed
  ${VISHNU_SOURCE_DIR}/UMS/src/sed
  ${VISHNU_SOURCE_DIR}/FMS/src/sed
  ${UTILVISHNU_SOURCE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}
  ${LIBJANSSON_INCLUDE_DIR}
  ${XMS_SED_SOURCE_DIR}
  )

add_library(test_zmq_helper
  ../DIET_client.cpp
  ../Annuary.cpp
  ../Server.cpp
  ../SeD.cpp
  ../utils.cpp
  ../zhelpers.cpp
  ../sslhelpers.cpp
  ${logger_SRCS}
  )

add_library(fake_zmq
  zmq.hpp
  fake.cpp
  )

target_link_libraries (test_zmq_helper
  zmq_helper
  fake_zmq
  ${Boost_LIBRARIES}
  vishnu-core
  ${OPENSSL_LIBRARIES}
  ${LIBJANSSON_LIB}
  )

# register tests
unit_test(LazyPirateUnitTests zmq_helper test_zmq_helper)
unit_test(AnnuaryUnitTests zmq_helper test_zmq_helper )
unit_test(ZMQServerUnitTests test_zmq_helper zmq_helper)
unit_test(DIET_clientUnitTests zmq_helper test_zmq_helper)
unit_test(utilsUnitTests zmq_helper test_zmq_helper)

