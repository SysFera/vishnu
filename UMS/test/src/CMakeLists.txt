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
  ${UMS_TEST_DIR}
  ${UMS_TEST_DIR}/../../process/
  )

file( TO_NATIVE_PATH ${TEST_CONFIG_UMS} TESTCONF )
file( TO_NATIVE_PATH ${TEST_SQL} TESTF )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/include/config.h )

ums_test(automTest)

add_executable(stresser
  stress.cpp)

target_link_libraries( stresser
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${LIBPQ_LIB}
  ${CLI_SOURCE_DIR}
  ${Boost_LIBRARIES}
  emf4cpp-UMS_Data
  database-vishnu
  exception-vishnu
  vishnu-ums-api
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
