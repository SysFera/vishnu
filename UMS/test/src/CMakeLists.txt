include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${UMS_API_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATABASE_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${ZMQ_INCLUDE_DIR}
  ${MOCK_INCLUDE_DIR}
  )

# Standard VISHNU TESTS variables
file(TO_NATIVE_PATH "${TEST_SQL_PATH}" TESTSQLPATH)
file(TO_NATIVE_PATH "${TEST_CONFIG_PATH}" TESTCONFIGPATH)
file(TO_NATIVE_PATH "${TEST_BIN_PATH}" TESTBINPATH)

set (TEST_DIET_SED_CONFIG "${TEST_CONFIG_PATH}/SeD_testing.cfg")
file(TO_NATIVE_PATH ${TEST_DIET_SED_CONFIG} DIETSEDCONF)
# Path to the VISHNU UMS SeD configuration
set (TEST_UMS_SED_CONFIG "${TEST_CONFIG_PATH}/umssed_testing.cfg")
file (TO_NATIVE_PATH ${TEST_UMS_SED_CONFIG} UMSSEDCONF)

# Specific to UMS
# Path to the sendmail script (used for umssed_testing.cfg)
set (TEST_SENDMAIL_SCRIPT "${UTILVISHNU_SOURCE_DIR}/sendmail.py" CACHE FILEPATH "Path to the sendmail script used for tests")
mark_as_advanced(TEST_SENDMAIL_SCRIPT)
file (TO_NATIVE_PATH ${TEST_SENDMAIL_SCRIPT} SENDMAILPY)

# Create header for VISHNU UMS tests
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/UMS_testconfig.h.in
  ${PROJECT_BINARY_DIR}/include/UMS_testconfig.h)
# Create configuration for VISHNU UMS SeD
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/umssed_testing.cfg.in
  ${TEST_CONFIG_PATH}/umssed_testing.cfg
)

# Create configuration for .netrc file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.netrc.in
  ${TEST_CONFIG_PATH}/.netrc
)


include(UMSTestingMacros)
ums_test_setup()
ums_test(ums_automTest)

add_executable(ums_stresser
  stress.cpp)

target_link_libraries(ums_stresser
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  vishnu-core
  vishnu-ums-client
  ${LIBCRYPT_LIB}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_subdirectory(sql)
