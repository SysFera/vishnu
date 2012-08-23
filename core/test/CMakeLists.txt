# Project: vishnu-test
# Author: B.Isnard
# Date: April 2011

if (BUILD_TESTING)

  #ZMQ is mandatory for running tests
  find_package(ZMQ)
#  if (NOT ZMQ_FOUND)
#    message("ZMQ installation was not found. Please provide ZMQ_DIR:")
#    set(ZMQ_DIR "" CACHE PATH "Root of ZMQ install tree.")
#  endif (ZMQ_FOUND)

  set(NAMER_INSTALL_DIR ${PROJECT_BINARY_DIR}/bin CACHE PATH
    "PATH to the ZeroMQ installation directory")

  #Boost::process is required
  include_directories(third-party)

  # test setup
  # for the moment, we only support in-place testing
  set(BIN_DIR ${PROJECT_BINARY_DIR}/bin)
  set(CONFIG_DIR ${TEST_CONFIG_PATH})
  set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})
  set(GENERATED_LOG_TESTS_DIR ${PROJECT_BINARY_DIR}/generated_test_logs)

  #temporary directories for Dagda
  set (TEST_DATA_DIR "/tmp" CACHE PATH "Path to the directory for test working directories")
  mark_as_advanced(TEST_DATA_DIR)
  file(TO_NATIVE_PATH "${TEST_DATA_DIR}/MA_test"     MA_DAGDA_DIR_NATIVE_PATH)
  file(TO_NATIVE_PATH "${TEST_DATA_DIR}/LA_test"     LA_DAGDA_DIR_NATIVE_PATH)
  file(TO_NATIVE_PATH "${TEST_DATA_DIR}/SeD_test"    SED_DAGDA_DIR_NATIVE_PATH)
  file(TO_NATIVE_PATH "${TEST_DATA_DIR}/Client_test" CLIENT_DAGDA_DIR_NATIVE_PATH)

  add_subdirectory(cfgs)
  add_subdirectory(src)

endif(BUILD_TESTING)
