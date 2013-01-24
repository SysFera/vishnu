include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${FMS_DATA_DIR}
  ${UMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATABASE_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/include
  ${CONFIG_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${UMS_TEST_SOURCE_DIR}
  )

file(TO_NATIVE_PATH "${TEST_BIN_PATH}" TESTBINPATH)



# Specific to FMS

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
fms_test(fms_automTest17)



add_executable(fms_stresser
  fms_stress.cpp)

target_link_libraries(fms_stresser
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  vishnu-core
  ${LIBCRYPT_LIB}
  vishnu-ums-client
  vishnu-fms-client)

