include_directories(client api
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_EMF_DATA_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}
  ${VERSION_MANAGER_SOURCE_DIR}
  ${ZMQ_INCLUDE_DIR}
  ${MOCK_INCLUDE_DIR})

set(client_SRCS client/SessionProxy.cpp
  client/AuthAccountProxy.cpp
  client/AuthSystemProxy.cpp
  client/UserProxy.cpp
  client/MachineProxy.cpp
  client/LocalAccountProxy.cpp
  client/ConfigurationProxy.cpp
  client/OptionValueProxy.cpp
  client/utilsClient.cpp
  client/UtilsProxy.cpp
  client/NetrcReader.cpp)

set(cli_SRCS cli/utils
  cli/daemon_cleaner
  cli/configureOptionUtils
  cli/localAccountUtils
  cli/userUtils
  cli/machineUtils
  cli/listMachineUtils
  cli/listHistoryCmdUtils
  cli/authenticationSystemUtils.cpp
  cli/authenticationAccountUtils.cpp)

set(api_SRCS api/api_ums.cpp)
set(api_HEADERS api/api_ums.hpp)

add_library(vishnu-ums-client ${client_SRCS} ${cli_SRCS} ${api_SRCS})
set_target_properties(vishnu-ums-client PROPERTIES VERSION ${VISHNU_VERSION})
target_link_libraries(vishnu-ums-client
  ${Boost_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core
  ${CMAKE_DL_LIBS}
  ${ZMQ_LIBRARIES}
  zmq_helper
)

install(TARGETS vishnu-ums-client DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})

## client CLI
add_subdirectory(cli)

if(NOT CLIENT_ONLY)
  include_directories(
  ${CONFIG_SOURCE_DIR}
  ${DATABASE_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${UMS_SERVER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${LDAP_PROXY_SOURCE_DIR}
  ${AUTHENTICATOR_INCLUDE_DIR})

  set(sed_SRCS sed/umssed.cpp
    sed/ServerUMS.cpp
    sed/internalApi.cpp
    sed/MonitorUMS.cpp)
  set(server_SRCS server/SessionServer.cpp
    server/UserServer.cpp
    server/MachineClientServer.cpp
    server/OptionValueServer.cpp
    server/MachineServer.cpp
    server/LocalAccountServer.cpp
    server/ConfigurationServer.cpp
    server/CommandServer.cpp
    server/AuthSystemServer.cpp
    server/AuthAccountServer.cpp
    )

  add_library(vishnu-ums-server ${server_SRCS})
  set_target_properties(vishnu-ums-server PROPERTIES VERSION ${VISHNU_VERSION})
  install(TARGETS vishnu-ums-server DESTINATION ${LIB_INSTALL_DIR})

  target_link_libraries(vishnu-ums-server vishnu-core vishnu-core-server ${CMAKE_DL_LIBS})

  add_executable(umssed ${sed_SRCS})
  target_link_libraries(umssed 
#${DIET_SERVER_LIBRARIES}
    ${LOG_LIBRARY}
    vishnu-ums-server
    emf4cpp-vishnu
    ${LIBCRYPT_LIB}
    ${LDAP_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${ZMQ_LIBRARIES}
    zmq_helper
)


install(TARGETS umssed DESTINATION ${SBIN_INSTALL_DIR})
endif()

