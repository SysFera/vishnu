include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${IMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${IMS_DATA_DIR}
  ${TMS_DATA_DIR}
  ${UMS_API_SOURCE_DIR}
  ${TMS_API_SOURCE_DIR}
  ${IMS_API_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${TMS_CLIENT_PROXY_SOURCE_DIR}
  ${IMS_CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATABASE_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${UMS_TEST_SOURCE_DIR}
  ${IMS_UTIL_DIR}
  )

# Standard VISHNU TESTS variables
file(TO_NATIVE_PATH "${TEST_SQL_PATH}" TESTSQLPATH)
file(TO_NATIVE_PATH "${TEST_CONFIG_PATH}" TESTCONFIGPATH)
file(TO_NATIVE_PATH "${TEST_BIN_PATH}" TESTBINPATH)

set (TEST_DIET_SED_CONFIG "${TEST_CONFIG_PATH}/SeD_testing.cfg")
file(TO_NATIVE_PATH ${TEST_DIET_SED_CONFIG} DIETSEDCONF)

# Path to the VISHNU IMS SeD configuration
set (TEST_IMS_SED_CONFIG "${TEST_CONFIG_PATH}/imssed_testing.cfg")
file (TO_NATIVE_PATH ${TEST_IMS_SED_CONFIG} IMSSEDCONF)

set (TEST_SENDMAIL_SCRIPT "${UTILVISHNU_SOURCE_DIR}/sendmail.py" CACHE FILEPATH "Path to the sendmail script used for tests")
mark_as_advanced(TEST_SENDMAIL_SCRIPT)
file (TO_NATIVE_PATH ${TEST_SENDMAIL_SCRIPT} SENDMAILPY)

# Specific to IMS
set(TEST_PATH_SCRIPT_REMOTE_UMSSED "${TEST_BIN_PATH}/launchRemoteUmssed.sh" CACHE FILEPATH "The script to launch the umssed on the remote machine")
mark_as_advanced(TEST_PATH_SCRIPT_REMOTE_UMSSED)
file(TO_NATIVE_PATH "${TEST_PATH_SCRIPT_REMOTE_UMSSED}" TESTPATHSCRIPTREMOTEUMSSED)
file(TO_NATIVE_PATH "${TEST_REMOTE_HOSTNAME}" TESTREMOTEHOSTNAME)

# Create header for VISHNU IMS tests
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/IMS_testconfig.h.in
  ${PROJECT_BINARY_DIR}/include/IMS_testconfig.h)
# Create configuration for VISHNU IMS SeD
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/imssed_testing.cfg.in
  ${TEST_CONFIG_PATH}/imssed_testing.cfg
)



include(IMSTestingMacros)
ims_test_setup()
ims_test(ims_automTest1)
ims_test(ims_automTest2)
ims_test(ims_automTest3)
ims_test(ims_automTest4)
ims_test(ims_automTest5)
ims_test(ims_automTest6)
ims_test(ims_automTest7)
ims_test(ims_automTest8)
ims_test(ims_automTest9)
ims_test(ims_automTest10)
ims_test(ims_automTest11)
ims_test(ims_automTest12)
ims_test(ims_automTest13)

add_executable(ims_stresser
 ims_stress.cpp)

target_link_libraries(ims_stresser
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  vishnu-core
  crypt
  vishnu-ums-client
  vishnu-ims-client)

add_subdirectory(sql)
