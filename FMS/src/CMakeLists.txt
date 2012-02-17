IF (WIN32)
include_directories(client api
  ${DIET_INCLUDE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR} 
  ${UMS_EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DEPS_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  )
ELSE (WIN32)
include_directories(client api
  ${DIET_INCLUDE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR} 
  ${UMS_EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  )
ENDIF (WIN32)
set(client_SRCS client/FileProxy
  client/LocalFileProxy
  client/RemoteFileProxy
  client/FileProxyFactory
  client/FileTransferProxy)

set(api_SRCS api/api_fms.cpp)
set(api_HEADERS api/api_fms.hpp)

add_library(vishnu-fms-client ${client_SRCS} ${api_SRCS})
SET_TARGET_PROPERTIES(vishnu-fms-client PROPERTIES VERSION ${VISHNU_VERSION})

IF (WIN32)
target_link_libraries(vishnu-fms-client
  ${DIET_CLIENT_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core
  vishnu-ums-client
  crypt32)
ELSE (WIN32)
target_link_libraries(vishnu-fms-client
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core
  crypt)
ENDIF (WIN32)

## client CLI
add_subdirectory(cli)

install(TARGETS vishnu-fms-client DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})

if(NOT CLIENT_ONLY)
  include_directories(${DATA_BASE_INCLUDE_DIR}
    ${CONFIG_SOURCE_DIR}
    ${REGISTRY_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${FMS_SERVER_SOURCE_DIR})

  set(sed_SRCS sed/fmssed.cpp
    sed/ServerFMS.cpp
    sed/internalApiFMS.cpp
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
    sed/MonitorFMS.cpp)

  set(server_SRCS
    server/File.cpp
    server/SSHFile.cpp
    server/FileFactory.cpp
    server/FileTransferCommand.cpp
    server/FileTransferServer.cpp
    server/ScpCommand.cpp
    server/RsyncCommand.cpp)

  add_executable(fmssed ${sed_SRCS} ${server_SRCS})
  install(TARGETS fmssed DESTINATION ${SBIN_INSTALL_DIR})
  target_link_libraries(fmssed ${DIET_SERVER_LIBRARIES}
    ${LOG_LIBRARY}
    ${DATABASE_LIBS}
    ${Boost_LIBRARIES}
    vishnu-core
    vishnu-core-server
    vishnu-ums-server
    emf4cpp-vishnu
    crypt)

endif()