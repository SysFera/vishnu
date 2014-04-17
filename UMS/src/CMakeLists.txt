include_directories(client api
  ${CONFIG_SOURCE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_EMF_DATA_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}
  ${VERSION_MANAGER_SOURCE_DIR}
  ${ZMQ_INCLUDE_DIR}
  ${COMMUNICATION_INCLUDE_DIR}
  ${VISHNU_SOURCE_DIR}/UMS/src/sed
  ${LIBJANSSON_INCLUDE_DIR}
  ${REGISTRY_SOURCE_DIR}
  ${XMS_SED_SOURCE_DIR}
  )

set(client_SRCS client/SessionProxy.cpp
  client/AuthAccountProxy.cpp
  client/AuthSystemProxy.cpp
  client/UserProxy.cpp
  client/MachineProxy.cpp
  client/LocalAccountProxy.cpp
  client/OptionValueProxy.cpp
  client/utilsClient.cpp
  client/UtilsProxy.cpp
  client/ObjectIdProxy.cpp
  client/PingerProxy.cpp
  client/ExporterProxy.cpp
  client/VersionProxy.cpp
  client/NetrcReader.cpp)

set(cli_SRCS cli/utils.cpp
  cli/daemon_cleaner.cpp
  cli/configureOptionUtils.cpp
  cli/localAccountUtils.cpp
  cli/userUtils.cpp
  cli/machineUtils.cpp
  cli/listMachineUtils.cpp
  cli/listHistoryCmdUtils.cpp
  cli/authenticationSystemUtils.cpp
  cli/authenticationAccountUtils.cpp)

set(api_SRCS api/api_ums.cpp api/api_comm.cpp)
set(api_HEADERS api/api_ums.hpp api/api_comm.hpp)

## client CLI
if(COMPILE_CLIENT_UMS)
  add_subdirectory(cli)
  
  add_library(vishnu-ums-client ${client_SRCS} ${cli_SRCS} ${api_SRCS})
  set_target_properties(vishnu-ums-client PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-ums-client
    ${Boost_LIBRARIES}
    emf4cpp-vishnu
    vishnu-core
    ${CMAKE_DL_LIBS}
    ${ZMQ_LIBRARIES}
    zmq_helper)
  install(TARGETS vishnu-ums-client DESTINATION ${LIB_INSTALL_DIR})
  install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})
endif()


if(COMPILE_SERVER_UMS)
  include_directories(
    ${DATA_BASE_INCLUDE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${REGISTRY_SOURCE_DIR}
    ${AUTHENTICATOR_INCLUDE_DIR})

  set(server_SRCS server/SessionServer.cpp
    server/UserServer.cpp
    server/MachineClientServer.cpp
    server/OptionValueServer.cpp
    server/MachineServer.cpp
    server/LocalAccountServer.cpp
    server/CommandServer.cpp
    server/ObjectIdServer.cpp
    server/AuthSystemServer.cpp
    server/AuthAccountServer.cpp
    server/ExportServer.cpp
    server/ExportFactory.cpp
    server/export/shell/ShellExporter.cpp
    server/export/shell/UMSShellMapper.cpp
    server/export/shell/TMSShellMapper.cpp
    server/export/shell/FMSShellMapper.cpp)

  add_library(vishnu-ums-server ${server_SRCS})
  set_target_properties(vishnu-ums-server PROPERTIES VERSION ${VISHNU_VERSION})
  install(TARGETS vishnu-ums-server DESTINATION ${LIB_INSTALL_DIR})

  target_link_libraries(vishnu-ums-server vishnu-core vishnu-core-server ${CMAKE_DL_LIBS})

endif(COMPILE_SERVER_UMS)

