include_directories(client api
  ${CONFIG_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR} 
  ${UMS_EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ZMQ_INCLUDE_DIR}
  ${COMMUNICATION_INCLUDE_DIR}
  ${VISHNU_SOURCE_DIR}/FMS/src/sed
  ${LIBJANSSON_INCLUDE_DIR}
)

set(utils_SRCS ${UTILVISHNU_SOURCE_DIR}/fmsUtils.cpp)

set(client_SRCS client/FileProxy
  client/LocalFileProxy
  client/RemoteFileProxy
  client/FileProxyFactory
  client/FileTransferProxy)

set(api_SRCS api/api_fms.cpp)
set(api_HEADERS api/api_fms.hpp)


## client CLI
if(COMPILE_CLIENT_FMS)
  add_subdirectory(cli)
  add_library(vishnu-fms-client ${client_SRCS} ${api_SRCS} ${utils_SRCS})
  SET_TARGET_PROPERTIES(vishnu-fms-client PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-fms-client
    ${Boost_LIBRARIES}
    emf4cpp-vishnu
    vishnu-core
    vishnu-ums-client
    ${LIBCRYPT_LIB}
    ${ZMQ_LIBRARIES}
    zmq_helper)
  install(TARGETS vishnu-fms-client DESTINATION ${LIB_INSTALL_DIR})
  install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})
endif()

if(COMPILE_SERVER_FMS)
  include_directories(${DATA_BASE_INCLUDE_DIR}
    ${REGISTRY_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${FMS_SERVER_SOURCE_DIR})

  set(sed_SRCS sed/fmssed.cpp
    sed/ServerFMS.cpp
    sed/getInfosSrv.cpp
    sed/getContentSrv.cpp
    sed/headSrv.cpp
    sed/tailSrv.cpp
    sed/mkdirSrv.cpp
    sed/mkfileSrv.cpp
    sed/rmdirSrv.cpp
    sed/rmfileSrv.cpp
    sed/chgrpSrv.cpp
    sed/chmodSrv.cpp
    sed/lsSrv.cpp
    sed/transferSrv.cpp
    sed/MonitorFMS.cpp
    ${COMMUNICATION_INCLUDE_DIR}/CommServer.cpp
    ${utils_SRCS})

  set(server_SRCS
    server/File.cpp
    server/SSHFile.cpp
    server/FileFactory.cpp
    server/FileTransferCommand.cpp
    server/FileTransferServer.cpp)

  add_executable(fmssed ${sed_SRCS} ${server_SRCS})
  install(TARGETS fmssed DESTINATION ${SBIN_INSTALL_DIR})
  target_link_libraries(fmssed 
    ${DATABASE_LIBS}
    ${Boost_LIBRARIES}
    vishnu-core
    vishnu-core-server
    vishnu-ums-server
    ${LDAP_LIBRARIES}
    emf4cpp-vishnu
    ${LIBCRYPT_LIB}
    ${ZMQ_LIBRARIES}
    zmq_helper)
endif(COMPILE_SERVER_FMS)
