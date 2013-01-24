include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
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
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${UMS_TEST_SOURCE_DIR}
  ${IMS_UTIL_DIR}
  )



set (TEST_SENDMAIL_SCRIPT "${UTILVISHNU_SOURCE_DIR}/sendmail.py" CACHE FILEPATH "Path to the sendmail script used for tests")
mark_as_advanced(TEST_SENDMAIL_SCRIPT)
file (TO_NATIVE_PATH ${TEST_SENDMAIL_SCRIPT} SENDMAILPY)


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
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  vishnu-core
  ${LIBCRYPT_LIB}
  vishnu-ums-client
  vishnu-ims-client)

