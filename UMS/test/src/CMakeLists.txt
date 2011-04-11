include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${API_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  )

set (TEST_CONFIG_UMS "${PROJECT_BINARY_DIR}/include")
file( TO_NATIVE_PATH ${TEST_CONFIG_UMS} TESTCONF )
set (TEST_SQL "${CMAKE_CURRENT_SOURCE_DIR}/sql")
file( TO_NATIVE_PATH ${TEST_SQL} TESTF )
# Path to the DIET configuration for the SeD (used for umssed_testing.cfg)
set (TEST_DIET_SED_CONFIG "${PROJECT_BINARY_DIR}/etc/SeD_testing.cfg")
file( TO_NATIVE_PATH ${TEST_DIET_SED_CONFIG} DIETSEDCONF)
# Path to the sendmail script (used for umssed_testing.cfg)
set (TEST_SENDMAIL_SCRIPT "${UTILVISHNU_SOURCE_DIR}/sendmail.py" CACHE FILEPATH "Path to the sendmail script used for tests")
mark_as_advanced(TEST_SENDMAIL_SCRIPT)
file ( TO_NATIVE_PATH ${TEST_SENDMAIL_SCRIPT} SENDMAILPY)
# Path to the VISHNU UMS SeD configuration
set (TEST_UMS_SED_CONFIG "${PROJECT_BINARY_DIR}/etc/umssed_testing.cfg")
file ( TO_NATIVE_PATH ${TEST_UMS_SED_CONFIG} UMSSEDCONF)
# Path to the VISHNU UMS SeD executable
file( TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/bin" UMSSEDBINDIR)

# Create header for VISHNU UMS tests
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/testconfig.h.in
  ${PROJECT_BINARY_DIR}/include/testconfig.h )
# Create configuration for VISHNU UMS SeD
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/umssed_testing.cfg.in
  ${PROJECT_BINARY_DIR}/etc/umssed_testing.cfg
)


include(UMSTestingMacros)
ums_test_setup()
ums_test(automTest)

add_executable(stresser
  stress.cpp)

target_link_libraries( stresser
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  database-vishnu
  exception-vishnu
  vishnu-ums-api
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
