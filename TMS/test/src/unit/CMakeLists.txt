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
  ${UMS_API_SOURCE_DIR}
  ${UMS_SERVER_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${VISHNU_SOURCE_DIR}/core/test/mock/database/
  ${VISHNU_SOURCE_DIR}/TMS/src/utils/
  ${VISHNU_SOURCE_DIR}/UMS/src/server/
  ${VISHNU_SOURCE_DIR}/TMS/src/sed/
  ${XMS_SED_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${VISHNU_SOURCE_DIR}/core/src/version
  )

include(UnitTest)

if (COMPILE_SERVERS)
unit_test(POSIXParserUnitTests vishnu-tms-posix vishnu-tms-server vishnu-core-server mockDb  )
unit_test(EnvUnitTests vishnu-tms-server mockDb vishnu-core-server)
unit_test(ScriptGenConvertorUnitTests vishnu-tms-server vishnu-core-server)


add_definitions(-DMODULE_PREFIX="${CMAKE_SHARED_MODULE_PREFIX}")
add_definitions(-DMODULE_SUFFIX="${CMAKE_SHARED_MODULE_SUFFIX}")

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
  ${COMMUNICATION_INCLUDE_DIR}/utils.cpp
  )
if(WIN32)
  set(server_mock_SRCS ${server_mock_SRCS} ${VISHNU_SOURCE_DIR}/TMS/src/utils/SharedLibraryImpl_win32.cc)
else()
  set(server_mock_SRCS ${server_mock_SRCS} ${VISHNU_SOURCE_DIR}/TMS/src/utils/SharedLibraryImpl_posix.cc)
endif()
add_library(vishnu-tms-server-mock ${server_mock_SRCS})
set_target_properties(vishnu-tms-server-mock PROPERTIES VERSION ${VISHNU_VERSION})
target_link_libraries(vishnu-tms-server-mock vishnu-ums-server-mock mockDb  ${LIBJANSSON_LIB})
set(sed_mock_SRCS ${VISHNU_SOURCE_DIR}/XMS/src/xmssed.cpp
  ${VISHNU_SOURCE_DIR}/XMS/src/ServerXMS.cpp
  ${VISHNU_SOURCE_DIR}/XMS/src//internalApiUMS.cpp
  ${VISHNU_SOURCE_DIR}/XMS/src/internalApiTMS.cpp
  ${VISHNU_SOURCE_DIR}/XMS/src/MonitorXMS.cpp
  ${COMMUNICATION_INCLUDE_DIR}/CommServer.cpp
  )
add_library(tmssed-mock ${sed_mock_SRCS})
target_link_libraries(tmssed-mock
  ${Boost_LIBRARIES}
  mockDb
  vishnu-core
  ${LIBCRYPT_LIB}
  vishnu-core-server
  vishnu-ums-server-mock
  vishnu-tms-server-mock
  vishnu-tms-client
  ${LDAP_LIBRARIES}
  emf4cpp-vishnu
  ${ZMQ_LIBRARIES}
  zmq_helper
  ${USED_BATCH_LIB})

unit_test(JobServerUnitTests vishnu-tms-server-mock vishnu-ums-server)
unit_test(ListJobServerUnitTests vishnu-tms-server-mock tmssed-mock vishnu-ums-server)
endif(COMPILE_SERVERS)
