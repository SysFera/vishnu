include_directories(client api utils
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR} 
  ${UMS_EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${UMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${SIGAR_INCLUDE_DIR}
  ${ZMQ_INCLUDE_DIR}
  ${MOCK_INCLUDE_DIR}
  ${VISHNU_SOURCE_DIR}/mockZMQ/ZMQHelper
  ${XMLRPC_INCLUDE_DIR}
)


set(client_SRCS
  client/MetricProxy.cpp
  client/ObjectIdProxy.cpp
  client/SysInfoProxy.cpp
  client/ThresholdProxy.cpp
  client/ProcessCtlProxy.cpp
  client/ExporterProxy.cpp)

set(cli_SRCS cli/displayer.cpp)
set(api_SRCS api/api_ims.cpp)
set(api_HEADERS api/api_ims.hpp)

add_library(vishnu-ims-client ${client_SRCS} ${cli_SRCS} ${api_SRCS})
set_target_properties(vishnu-ims-client PROPERTIES VERSION ${VISHNU_VERSION})
target_link_libraries(vishnu-ims-client
  ${Boost_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core
  ${LIBCRYPT_LIB}
)
if(COMPILE_FMS)
  target_link_libraries(vishnu-ims-client vishnu-fms-client)
endif()

install(TARGETS vishnu-ims-client DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})

## client CLI
add_subdirectory(cli)

if(NOT CLIENT_ONLY)
  include_directories(${DATA_BASE_INCLUDE_DIR}
    ${CONFIG_SOURCE_DIR}
    ${REGISTRY_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${IMS_SERVER_SOURCE_DIR})
  
  set(sed_SRCS sed/imssed.cpp
    sed/ServerIMS.cpp
    sed/internalApiIMS.cpp)
  set(server_SRCS 
    server/controller/HM/HM.cpp
    server/controller/process/ProcessCtl.cpp
    server/data/ProcessServer.cpp
    server/data/MetricServer.cpp
    server/data/ExportServer.cpp
    server/data/ExportFactory.cpp
    server/data/export/shell/ShellExporter.cpp
    server/data/export/shell/UMSShellMapper.cpp
    server/data/export/shell/TMSShellMapper.cpp
    server/data/export/shell/FMSShellMapper.cpp
    server/data/export/shell/IMSShellMapper.cpp
    server/data/ObjectIdServer.cpp
    server/data/SysInfoServer.cpp
    server/data/ThresholdServer.cpp)

  add_executable(imssed ${sed_SRCS} ${server_SRCS})
  target_link_libraries(imssed
    ${DATABASE_LIBS}
    ${Boost_LIBRARIES}
    vishnu-core
    vishnu-core-server
    vishnu-ums-server
    ${LDAP_LIBRARIES}
    emf4cpp-vishnu
    ${LIBCRYPT_LIB}
    ${SIGAR_LIBRARIES}
    ${ZMQ_LIBRARIES}
    zmq_helper
    ${XMLRPC_LIBRARIES}
)

  install(TARGETS imssed DESTINATION ${SBIN_INSTALL_DIR})
endif (NOT CLIENT_ONLY)  
