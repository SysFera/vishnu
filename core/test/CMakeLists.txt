# Project: vishnu-test
# Author: B.Isnard
# Date: April 2011

if( BUILD_TESTING )

  #OmniORB is mandatory for running tests
  find_package(OmniORB)
  if (OMNIORB4_FOUND)
    add_definitions(-D__OMNIORB4__)
  else (OMNIORB4_FOUND)
    message("omniORB installation was not found. Please provide OMNIORB4_DIR:")
    set(OMNIORB4_DIR "" CACHE PATH "Root of omniORB install tree.")
  endif (OMNIORB4_FOUND)

  set( OMNINAMES_LOGDIR ${CMAKE_CURRENT_BINARY_DIR}/log
    CACHE PATH "PATH to the omniNames directory" )
  set( OMNIORB_INSTALL_DIR "" CACHE PATH
    "PATH to the omniORB installation directory" )
  set( LOCAL_TEST_PORT "2809" CACHE STRING "Test port for the omniNames" )

  file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/log )

  #Boost::process is required
  include_directories(third-party)

  # test setup
  # for the moment, we only support in-place testing
  set( BIN_DIR ${PROJECT_BINARY_DIR}/bin )
  set( CONFIG_DIR ${PROJECT_BINARY_DIR}/etc )
  set( EXECUTABLE_OUTPUT_PATH ${BIN_DIR} )
  set( GENERATED_LOG_TESTS_DIR ${PROJECT_BINARY_DIR}/generated_test_logs )

  #temporary directories for Dagda
  file( TO_NATIVE_PATH "/tmp/MA_test"     MA_DAGDA_DIR_NATIVE_PATH )
  file( TO_NATIVE_PATH "/tmp/LA_test"     LA_DAGDA_DIR_NATIVE_PATH )
  file( TO_NATIVE_PATH "/tmp/SeD_test"    SED_DAGDA_DIR_NATIVE_PATH )
  file( TO_NATIVE_PATH "/tmp/Client_test" CLIENT_DAGDA_DIR_NATIVE_PATH )

  add_subdirectory(cfgs)
  add_subdirectory(src)

endif( BUILD_TESTING )
