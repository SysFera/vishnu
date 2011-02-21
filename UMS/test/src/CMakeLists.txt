file( TO_NATIVE_PATH ${TEST_CONFIG_UMS} TESTCONF )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/include/config.h )

ums_test(automTest)
