include_directories(client api utils
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR} 
  ${UMS_EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${UMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR})


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
target_link_libraries(vishnu-ims-client
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core
  crypt)
if(COMPILE_FMS)
  target_link_libraries(vishnu-ims-client vishnu-fms-client)
endif()

install(TARGETS vishnu-ims-client DESTINATION lib)
install(FILES {api_HEADERS} DESTINATION include)

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
  set(server_SRCS server/controller/logTool/LogTool.cpp
    server/controller/logTool/IMSVishnuTool.cpp
    server/controller/logTool/Watcher.cpp
    server/controller/logTool/ToolFactory.cpp
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
  target_link_libraries(imssed ${DIET_SERVER_LIBRARIES}
    ${LOG_LIBRARY}
    ${DATABASE_LIBS}
    ${Boost_LIBRARIES}
    vishnu-core
    vishnu-core-server
    vishnu-ums-server
    emf4cpp-vishnu
    crypt)

  install(TARGETS imssed DESTINATION sbin)
endif (NOT CLIENT_ONLY)  
