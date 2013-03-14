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
  ${FMS_API_SOURCE_DIR}
  ${IMS_API_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${TMS_CLIENT_PROXY_SOURCE_DIR}
  ${IMS_CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${CONFIG_SOURCE_DIR}
  ${UMS_TEST_SOURCE_DIR}
  ${FMS_TEST_SOURCE_DIR}
  ${TMS_TEST_SOURCE_DIR}
  ${IMS_UTIL_DIR}
  ${VISHNU_SOURCE_DIR}/UMS/src/sed
  ${VISHNU_SOURCE_DIR}/communication
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

ims_test(ims_automTest12)
ims_test(ims_automTest13)

if (COMPILE_TMS)
  ims_test(ims_automTest11)

  # we cannot use set_source_files_properties here, as it only
  # works on targets defined in this CMakeLists.txt
  # (here defined by the macro ims_test)
  add_definitions("-DCOMPILE_TMS")
  target_link_libraries(ims_automTest11 vishnu-tms-client)
  target_link_libraries(ims_automTest7 vishnu-tms-client)
  target_link_libraries(ims_automTest12 vishnu-tms-client)
endif (COMPILE_TMS)



## Currently disabled, not maintained since a while
# add_executable(ims_stresser ims_stress.cpp)

# target_link_libraries(ims_stresser
#   ${LIBPQ_LIB}
#   ${Boost_LIBRARIES}
#   vishnu-core
#   ${LIBCRYPT_LIB}
#   vishnu-ums-client
#   vishnu-ims-client)

