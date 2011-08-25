include_directories(client api)

set(client_SRCS client/SessionProxy.cpp
  client/UserProxy.cpp
  client/MachineProxy.cpp
  client/LocalAccountProxy.cpp
  client/ConfigurationProxy.cpp
  client/OptionValueProxy.cpp
  client/utilsClient.cpp
  client/UtilsProxy.cpp)

set(cli_SRCS cli/utils
  cli/daemon_cleaner
  cli/configureOptionUtils
  cli/localAccountUtils
  cli/userUtils
  cli/machineUtils
  cli/listMachineUtils
  cli/listHistoryCmdUtils)

set(api_SRCS api/api_ums.cpp)
set(api_HEADERS api/api_ums.hpp)

add_library(vishnu-ums-client ${client_SRCS} ${cli_SRCS} ${api_SRCS})
set_target_properties(vishnu-ums-client PROPERTIES VERSION ${VISHNU_VERSION})
target_link_libraries(vishnu-ums-client
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core)

install(TARGETS vishnu-ums-client DESTINATION lib)
install(FILES ${api_HEADERS} DESTINATION include)

## client CLI
add_subdirectory(cli)

if(NOT CLIENT_ONLY)
  include_directories(
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${UMS_SERVER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR})

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
    server/ListUsersServer.cpp)

  add_library(vishnu-ums-server ${server_SRCS})
  set_target_properties(vishnu-ums-server PROPERTIES VERSION ${VISHNU_VERSION})
  install(TARGETS vishnu-ums-server DESTINATION lib)
  add_executable(umssed ${sed_SRCS})
  target_link_libraries(umssed ${DIET_SERVER_LIBRARIES}
    ${Boost_LIBRARIES}
    ${DATABASE_LIBS}
    vishnu-core
    vishnu-core-server
    vishnu-ums-server
    emf4cpp-vishnu
    crypt)

  install(TARGETS umssed DESTINATION sbin)
endif()

