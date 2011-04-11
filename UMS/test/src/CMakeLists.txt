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
set (TEST_SQL "${UMS_TEST_DIR}/src/sql")
file( TO_NATIVE_PATH ${TEST_SQL} TESTF )
set (TEST_UMS_SED_CONFIG "${PROJECT_BINARY_DIR}/etc/SeD_testing.cfg")
file( TO_NATIVE_PATH ${TEST_UMS_SED_CONFIG} UMSSEDCONF)
file( TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/bin" UMSSEDBINDIR)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/testconfig.h.in
  ${PROJECT_BINARY_DIR}/include/testconfig.h )


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
