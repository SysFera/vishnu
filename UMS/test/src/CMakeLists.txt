include_directories(
  ${UTILVISHNU_SOURCE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${UMS_API_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/include
  ${TESTVISHNU_SOURCE_DIR}
  ${TESTVISHNU_SOURCE_DIR}/../third-party
  ${ZMQ_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}
  ${LIBJANSSON_INCLUDE_DIR}
  ${COMMUNICATION_INCLUDE_DIR}
  )



include(UMSTestingMacros)
ums_test_setup()
ums_test(ums_automTest1)
ums_test(ums_automTest2)
ums_test(ums_automTest3)
ums_test(ums_automTest4)
ums_test(ums_automTest5)
ums_test(comm_automTest)


# Unit tests
add_subdirectory(unit)


## Currently disabled, not maintained since a while
# add_executable(ums_stresser stress.cpp)

# target_link_libraries(ums_stresser
#   ${LIBPQ_LIB}
#   ${Boost_LIBRARIES}
#   vishnu-core
#   vishnu-ums-client
#   ${LIBCRYPT_LIB}
#   ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

