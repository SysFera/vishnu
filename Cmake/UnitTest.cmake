
###############################################################################
# unit_test_setup: macro that setup our test environment
# it add a test-xml target
###############################################################################

macro( unit_test_setup )
  enable_testing()
  # add a test-xml targets that allows us to generate Boost.Test xml reports
  add_custom_target( unit_test-xml )
  include(CTest)

  file( MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/reports )
  set( REPORT_OUTPUT_PATH ${PROJECT_BINARY_DIR}/reports )
endmacro()


###############################################################################
# unit_test: macro that setup a test
# @param[in] NAME  test filename stripped from extension
# @param[in] LIBS (optional) list of libraries to link the test with
###############################################################################

function( unit_test NAME )
  # create unit tests executable
  add_executable( ${NAME}
    "${NAME}.cpp"
    #entry point
    TestRunner.cpp )
  include_directories( ${Boost_INCLUDE_DIRS}
    )
  
  # link libraries
  target_link_libraries( ${NAME}
    ${Boost_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})


  foreach(LIBT ${ARGN})
    target_link_libraries( ${NAME} ${LIBT} )
  endforeach()
  
  # test executable installation has not been tested yet -sic-
  # install( TARGETS ${NAME} DESTINATION bin )
  add_test( ${NAME} ${BIN_DIR}/${NAME} )
  
  # prevent Boost.Test to catch unrelated exceptions
  set_property( TEST ${NAME}
    PROPERTY ENVIRONMENT "BOOST_TEST_CATCH_SYSTEM_ERRORS=no;" )
  # just make sure that our test are run in a serial fashion
  set_property( TEST ${NAME} PROPERTY RUN_SERIAL ON )
  
  #
  add_custom_target( ${NAME}-xml
    COMMAND ${CMAKE_COMMAND}
    -DTEST_PROG=${NAME}
    -DBIN_PATH=${BIN_DIR}
    -DREPORT_PATH=${REPORT_OUTPUT_PATH}
    -P ${PROJECT_SOURCE_DIR}/Cmake/runtest.cmake )
  add_dependencies( unit_test-xml ${NAME}-xml )
endfunction()

###############################################################################
