include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${TMS_DATA_DIR}
  ${API_SOURCE_DIR}
  ${TMS_API_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${TMS_CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/include
  ${TMS_TEST_DIR}
  ${TMS_TEST_DIR}/../../process/
  )

file( TO_NATIVE_PATH ${TEST_CONFIG_TMS} TESTCONF )
#file( TO_NATIVE_PATH ${TEST_TMS_SQL} TESTF )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/include/config.h )

include(TMSTestingMacros)
#tms_test_setup()

tms_test(tms_automTest)

#add_executable(stresser
#  stress.cpp)

#target_link_libraries( stresser
#  ${DIET_CLIENT_LIB}
#  ${DIET_SERVER_LIB}
#  ${LIBPQ_LIB}
#  #  ${CLI_SOURCE_DIR}
#  ${Boost_LIBRARIES}
#  emf4cpp-UMS_Data
#  database-vishnu
  #  exception-vishnu
  #  vishnu-ums-api
  #  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
