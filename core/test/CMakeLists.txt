# Project: vishnu-test
# Author: B.Isnard
# Date: April 2011

if (BUILD_TESTING)

  #ZMQ is mandatory for running tests
  find_package(ZMQ)
  set(DISPATCHER_INSTALL_DIR ${PROJECT_BINARY_DIR}/bin CACHE PATH
       "PATH to the ZeroMQ installation directory")

  #Boost::process is required
  include_directories(third-party)

  # test setup
  # for the moment, we only support in-place testing
  set(BIN_DIR ${PROJECT_BINARY_DIR}/bin)
  set(CONFIG_DIR ${TEST_CONFIG_PATH})
  set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})
  set(GENERATED_LOG_TESTS_DIR ${PROJECT_BINARY_DIR}/generated_test_logs)

  add_subdirectory(cfgs)
  add_subdirectory(src)

  #set(TEST_DISPATCHER_CFG "${TEST_CONFIG_PATH}/dispatcher.cfg" CACHE PATH "Path to the dispatcher's configuration file")
  #file(TO_NATIVE_PATH "${TEST_DISPATCHER_CFG}" TEST_DISPATCHER_CONFIG)
  #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dispatcher.cfg.in ${TEST_DISPATCHER_CONFIG})
  
  #set(TEST_DISPATCHER_INIT_CFG "${TEST_CONFIG_PATH}/dispatcher_init.cfg" CACHE PATH "Path to the dispatcher's init configuration")
  #file(TO_NATIVE_PATH "${TEST_DISPATCHER_INIT_CFG}" TEST_DISPATCHER_INIT_CONFIG)
  #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dispatcher_init.cfg.in ${TEST_DISPATCHER_INIT_CONFIG})

endif(BUILD_TESTING)
