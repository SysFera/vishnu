include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${TMS_DATA_DIR}
  ${UMS_API_SOURCE_DIR}
  ${TMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${TMS_CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATABASE_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${UMS_TEST_SOURCE_DIR}
  ${FMS_TEST_SOURCE_DIR}
  ${VISHNU_SOURCE_DIR}/mockZMQ/ZMQHelper
  )

# Standard VISHNU TESTS variables
file(TO_NATIVE_PATH "${TEST_SQL_PATH}" TESTSQLPATH)
file(TO_NATIVE_PATH "${TEST_CONFIG_PATH}" TESTCONFIGPATH)
file(TO_NATIVE_PATH "${TEST_BIN_PATH}" TESTBINPATH)

set(TEST_CONFIG_TMS "${PROJECT_BINARY_DIR}/include")
file(TO_NATIVE_PATH ${TEST_CONFIG_TMS} TESTCONF)
set(TEST_TMS_SQL "${PROJECT_BINARY_DIR}/test_files/sql")
file(TO_NATIVE_PATH ${TEST_TMS_SQL} TMSSQL)
set(TEST_TMS_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
file(TO_NATIVE_PATH ${TEST_TMS_SCRIPTS} TMSSCRIPTS)
# Path to the DIET configuration for the SeD(used for umssed_testing.cfg)
set(TEST_DIET_SED_CONFIG "${TEST_CONFIG_PATH}/SeD_testing.cfg")
file(TO_NATIVE_PATH ${TEST_DIET_SED_CONFIG} DIETSEDCONF)
# Path to the VISHNU TMS SeD configuration
set(TEST_TMS_SED_CONFIG "${TEST_CONFIG_PATH}/tmssed_testing.cfg")
file(TO_NATIVE_PATH ${TEST_TMS_SED_CONFIG} TMSSEDCONF)
set(TEST_TMS_SED_CONFIG2 "${TEST_CONFIG_PATH}/tmssed_testing2.cfg")
file(TO_NATIVE_PATH ${TEST_TMS_SED_CONFIG2} TMSSEDCONF2)

# Path to the VISHNU TMS SeD executable
file(TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/bin" TMSSEDBINDIR)

# Create header for VISHNU TMS tests
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TMS_testconfig.h.in
  ${PROJECT_BINARY_DIR}/include/TMS_testconfig.h)
# Create configuration for the first TMS SeD
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tmssed_testing.cfg.in
  ${TEST_CONFIG_PATH}/tmssed_testing.cfg)
# Create configuration for the second TMS SeD
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tmssed_testing.cfg2.in
  ${TEST_CONFIG_PATH}/tmssed_testing2.cfg)

include(TMSTestingMacros)
tms_test_setup()
tms_test(tms_automTest1)
tms_test(tms_automTest2)
tms_test(tms_automTest3)
tms_test(tms_automTest4)
tms_test(tms_automTest5)
tms_test(tms_automTest6)
tms_test(tms_automTest7)
tms_test(tms_automTest8)

add_executable(tms_stresser
 tms_stress.cpp)

target_link_libraries(tms_stresser
 ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  vishnu-core
  ${LIBCRYPT_LIB}
  vishnu-ums-client
  vishnu-tms-client)

add_subdirectory(sql)
