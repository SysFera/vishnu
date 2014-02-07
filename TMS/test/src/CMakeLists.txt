include_directories(
  ${COMMUNICATION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
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
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${UMS_TEST_SOURCE_DIR}
  ${FMS_TEST_SOURCE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${LIBJANSSON_INCLUDE_DIR}
  ${REGISTRY_SOURCE_DIR}
  )


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


# Unit tests
add_subdirectory(unit)



## Currently disabled, not maintained since a while
# add_executable(tms_stresser tms_stress.cpp)
# target_link_libraries(tms_stresser
#   ${LIBPQ_LIB}
#   ${Boost_LIBRARIES}
#   vishnu-core
#   ${LIBCRYPT_LIB}
#   vishnu-ums-client
#   vishnu-tms-client)


