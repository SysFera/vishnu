include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${FMS_DATA_DIR}
  ${API_SOURCE_DIR}
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

set( TEST_FMS_HOST1 "localhost" CACHE STRING " hostname of file server 1")
set( TEST_FMS_HOST2 "localhost" CACHE STRING " hostname of file server 2")
mark_as_advanced( TEST_FMS_HOST1 )
mark_as_advanced( TEST_FMS_HOST2 )

set (TEST_CONFIG_FMS "${PROJECT_BINARY_DIR}/include")
file( TO_NATIVE_PATH ${TEST_CONFIG_FMS} TESTCONF )
set (TEST_FMS_SQL "${PROJECT_BINARY_DIR}/test_files/sql")
file( TO_NATIVE_PATH ${TEST_FMS_SQL} FMSSQL )
# Path to the DIET configuration for the SeD (used for tmssed_testing.cfg)
set (TEST_DIET_SED_CONFIG "${PROJECT_BINARY_DIR}/etc/SeD_testing.cfg")
file( TO_NATIVE_PATH ${TEST_DIET_SED_CONFIG} DIETSEDCONF)
# Path to the VISHNU UMS SeD configuration
set (TEST_FMS_SED_CONFIG "${PROJECT_BINARY_DIR}/etc/fmssed_testing.cfg")
file ( TO_NATIVE_PATH ${TEST_FMS_SED_CONFIG} FMSSEDCONF)
# Path to the VISHNU FMS SeD executable
file( TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/bin" FMSSEDBINDIR)

# Create header for VISHNU FMS tests
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/FMS_testconfig.h.in
  ${PROJECT_BINARY_DIR}/include/FMS_testconfig.h )
# Create configuration for VISHNU FMS SeD
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/fmssed_testing.cfg.in
  ${PROJECT_BINARY_DIR}/etc/fmssed_testing.cfg
)

include(FMSTestingMacros)
fms_test_setup()
fms_test(fms_automTest)

# add_executable(tms_stresser
#  tms_stress.cpp)

# target_link_libraries( tms_stresser
#  ${DIET_CLIENT_LIB}
#   ${DIET_SERVER_LIB}
#   ${LIBPQ_LIB}
#   ${Boost_LIBRARIES}
#   emf4cpp-UMS_Data
#   emf4cpp-FMS_Data
#   database-vishnu
#     exception-vishnu
#     vishnu-ums-api
#     vishnu-tms-api)

add_subdirectory( sql )
