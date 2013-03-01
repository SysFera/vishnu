include_directories(
  ${VISHNU_SOURCE_DIR}/TMS/src/posix_parser
  ${VISHNU_SOURCE_DIR}/TMS/src/server
)


include(UnitTest)
if(COMPILE_CLIENT_CLI AND COMPILE_SERVERS)
  unit_test(POSIXParserUnitTests ${POSIX_TEST_LIB})
  unit_test(EnvUnitTests vishnu-tms-server)


  unit_test(ScriptGenConvertorUnitTests vishnu-tms-server )


  add_definitions(-DMODULE_PREFIX="${CMAKE_SHARED_MODULE_PREFIX}")
  add_definitions(-DMODULE_SUFFIX="${CMAKE_SHARED_MODULE_SUFFIX}")
  include_directories(
    ${VISHNU_SOURCE_DIR}/TMS/src/posix_parser
    ${VISHNU_SOURCE_DIR}/TMS/src/server
    ${EMF4CPP_INCLUDE_DIR}
    ${EMF_DATA_DIR}
    ${UMS_EMF_DATA_DIR}
    ${TMS_EMF_DATA_DIR}
    ${UMS_CLIENT_PROXY_SOURCE_DIR}
    ${UTILVISHNU_SOURCE_DIR}
    ${VISHNU_EXCEPTION_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}/include/
    ${ZMQ_INCLUDE_DIR}
    ${COMMUNICATION_INCLUDE_DIR}
    ${UMS_API_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${FMS_API_SOURCE_DIR}
    ${CONFIG_SOURCE_DIR}
    ${VISHNU_SOURCE_DIR}/core/test/mock/database/
    ${VISHNU_SOURCE_DIR}/TMS/src/utils/
    ${VISHNU_SOURCE_DIR}/UMS/src/server/
  )

  set(server_mock_SRCS
      ${VISHNU_SOURCE_DIR}/TMS/src/server/BatchServer.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/server/SSHJobExec.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/server/JobServer.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/server/BatchFactory.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/server/ListQueuesServer.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/server/JobOutputServer.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/server/ScriptGenConvertor.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/server/WorkServer.cpp
      ${VISHNU_SOURCE_DIR}/TMS/src/utils/SharedLibrary.cc
      ${UTILVISHNU_SOURCE_DIR}/tmsUtils.cpp
      )
  if(WIN32)
    set(server_mock_SRCS ${server_mock_SRCS} ${VISHNU_SOURCE_DIR}/TMS/src/utils/SharedLibraryImpl_win32.cc)
  else()
    set(server_mock_SRCS ${server_mock_SRCS} ${VISHNU_SOURCE_DIR}/TMS/src/utils/SharedLibraryImpl_posix.cc)
  endif()
  add_library(vishnu-tms-server-mock ${server_mock_SRCS})
  set_target_properties(vishnu-tms-server-mock PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-tms-server-mock vishnu-ums-server-mock mockDb)
  install(TARGETS vishnu-tms-server-mock DESTINATION ${LIB_INSTALL_DIR})

  unit_test(JobServerUnitTests vishnu-tms-server-mock)
  #unit_test(ParserUnitTests vishnu-tms-server)

endif()
