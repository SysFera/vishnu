include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${FMS_DATA_DIR}
  ${UMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
#   ${CLIENT_PROXY_SOURCE_DIR}
#   ${FMS_CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${UMS_TEST_SOURCE_DIR}
  )

# Standard VISHNU TESTS variables
file(TO_NATIVE_PATH "${TEST_SQL_PATH}" TESTSQLPATH)
file(TO_NATIVE_PATH "${TEST_CONFIG_PATH}" TESTCONFIGPATH)
file(TO_NATIVE_PATH "${TEST_BIN_PATH}" TESTBINPATH)

set (TEST_DIET_SED_CONFIG "${TEST_CONFIG_PATH}/SeD_testing.cfg")
file(TO_NATIVE_PATH ${TEST_DIET_SED_CONFIG} DIETSEDCONF)
# Path to the VISHNU UMS SeD configuration
set (TEST_FMS_SED_CONFIG "${TEST_CONFIG_PATH}/fmssed_testing.cfg")
file (TO_NATIVE_PATH ${TEST_FMS_SED_CONFIG} FMSSEDCONF)

# Specific to FMS
set(TEST_FMS_HOST1 "localhost" CACHE STRING " hostname of file server 1")
set(TEST_FMS_HOST2 "localhost" CACHE STRING " hostname of file server 2")
set(TEST_FMS_DIR1 "/tmp" CACHE STRING "directory for file tests on server 1")
set(TEST_FMS_DIR2 "/tmp" CACHE STRING "directory for file tests on server 2")
mark_as_advanced(TEST_FMS_HOST1)
mark_as_advanced(TEST_FMS_HOST2)
mark_as_advanced(TEST_FMS_DIR1)
mark_as_advanced(TEST_FMS_DIR2)
file(TO_NATIVE_PATH ${TEST_FMS_DIR1} FMSDIR1)
file(TO_NATIVE_PATH ${TEST_FMS_DIR2} FMSDIR2)

# Create header for VISHNU FMS tests
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/FMS_testconfig.h.in
  ${PROJECT_BINARY_DIR}/include/FMS_testconfig.h)
# Create configuration for VISHNU FMS SeD
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fmssed_testing.cfg.in
  ${TEST_CONFIG_PATH}/fmssed_testing.cfg
)

include(FMSTestingMacros)
fms_test_setup()
fms_test(fms_automTest1)
fms_test(fms_automTest2)
fms_test(fms_automTest3)
fms_test(fms_automTest4)
fms_test(fms_automTest5)
fms_test(fms_automTest6)
fms_test(fms_automTest7)
fms_test(fms_automTest8)
fms_test(fms_automTest9)
fms_test(fms_automTest10)
fms_test(fms_automTest11)
fms_test(fms_automTest12)
fms_test(fms_automTest13)
fms_test(fms_automTest14)
fms_test(fms_automTest15)
fms_test(fms_automTest16)



add_executable(fms_stresser
  fms_stress.cpp)

target_link_libraries(fms_stresser
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  vishnu-core
  crypt
  vishnu-ums-client
  vishnu-fms-client)

add_subdirectory(sql)
