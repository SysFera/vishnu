include_directories(client api
  ${DIET_INCLUDE_DIR}
  ${OMNIORB4_INCLUDE_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${EMF_DATA_DIR} 
  ${UMS_EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)


set(client_SRCS client/JobProxy.cpp
  client/JobOutputProxy.cpp)

set(cli_SRCS cli/displayer.cpp)

set(api_SRCS api/api_tms.cpp)
set(api_HEADERS api/api_tms.hpp)

add_library(vishnu-tms-client ${client_SRCS} ${cli_SRCS} ${api_SRCS})
target_link_libraries(vishnu-tms-client
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core
  vishnu-ums-client
  ${LIBCRYPT_LIB})

install(TARGETS vishnu-tms-client DESTINATION ${LIB_INSTALL_DIR})
set_target_properties(vishnu-tms-client PROPERTIES VERSION ${VISHNU_VERSION})
install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})


add_subdirectory(cli)

if (NOT CLIENT_ONLY)
  include(config.cmake)
  include_directories(server
    ${DATA_BASE_INCLUDE_DIR}
    ${CONFIG_SOURCE_DIR}
    ${REGISTRY_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${FMS_SERVER_SOURCE_DIR}
    ${USED_BATCH_INCLUDE_DIR})
  
  set(slave_SRCS slave/slave.cpp)
  add_executable(tmsSlave ${slave_SRCS})
  target_link_libraries(tmsSlave
    ${Boost_LIBRARIES}
    ${DATABASE_LIBS}
    vishnu-core
    ${LIBCRYPT_LIB}
    vishnu-core-server
    vishnu-ums-server
    vishnu-tms-server
    emf4cpp-vishnu)

  set(sed_SRCS sed/tmssed.cpp
    sed/ServerTMS.cpp
    sed/internalApi.cpp
    sed/MonitorTMS.cpp)

  set(server_SRCS ${UsedBatchServer}
    server/BatchServer.cpp
    server/SSHJobExec.cpp
    server/JobServer.cpp
    server/BatchFactory.cpp
    server/ListQueuesServer.cpp
    server/JobOutputServer.cpp
    server/ScriptGenConvertor.cpp
    server/Env.cpp)
  message("coucou server_SRCS = ${server_SRCS}")
  message("coucou USED_BATCH_LIB = ${USED_BATCH_LIB}")
  add_library(vishnu-tms-server ${server_SRCS})
  set_target_properties(vishnu-tms-server PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-tms-server ${USED_BATCH_LIB} vishnu-ums-server)
  install(TARGETS vishnu-tms-server DESTINATION ${LIB_INSTALL_DIR})
  add_executable(tmssed ${sed_SRCS})
  target_link_libraries(tmssed ${DIET_SERVER_LIBRARIES}
    ${Boost_LIBRARIES}
    ${LOG_LIBRARY}
    ${DATABASE_LIBS}
    vishnu-core
    ${LIBCRYPT_LIB}
    vishnu-core-server
    vishnu-ums-server
    vishnu-tms-server
    emf4cpp-vishnu
    ${USED_BATCH_LIB})
  install(TARGETS tmsSlave DESTINATION ${SBIN_INSTALL_DIR}) 
  install(TARGETS tmssed DESTINATION ${SBIN_INSTALL_DIR})
endif (NOT CLIENT_ONLY)

