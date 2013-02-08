include_directories(client api
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
  ${MOCK_INCLUDE_DIR}
  ${UMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${VISHNU_SOURCE_DIR}/mockZMQ/ZMQHelper
  ${CONFIG_SOURCE_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/server/SGEConfig.hpp.in
  ${PROJECT_BINARY_DIR}/include/SGEConfig.hpp)

set(client_SRCS client/JobProxy.cpp
  client/JobOutputProxy.cpp
  client/WorkProxy.cpp
  client/tmsClientUtils.cpp
)

set(cli_SRCS cli/displayer.cpp)

set(api_SRCS api/api_tms.cpp)
set(api_HEADERS api/api_tms.hpp)

add_library(vishnu-tms-client ${client_SRCS} ${cli_SRCS} ${api_SRCS})
target_link_libraries(vishnu-tms-client
  ${Boost_LIBRARIES}
  emf4cpp-vishnu
  vishnu-core
  vishnu-ums-client
  vishnu-fms-client
  ${LIBCRYPT_LIB}
  ${ZMQ_LIBRARIES}
  zmq_helper
)

install(TARGETS vishnu-tms-client DESTINATION ${LIB_INSTALL_DIR})
set_target_properties(vishnu-tms-client PROPERTIES VERSION ${VISHNU_VERSION})
install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})


## client CLI
if(COMPILE_CLIENT_CLI)
  add_subdirectory(cli)
endif()


if (COMPILE_SERVERS)
  include(config.cmake)
  include_directories(server
    utils
    posix_parser
    ${DATA_BASE_INCLUDE_DIR}
    ${REGISTRY_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${FMS_SERVER_SOURCE_DIR}
    ${USED_BATCH_INCLUDE_DIR}
    ${TESTVISHNU_SOURCE_DIR}/../third-party)
  
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
    emf4cpp-vishnu
    ${USED_BATCH_LIB})

  set(sed_SRCS sed/tmssed.cpp
    sed/ServerTMS.cpp
    sed/internalApi.cpp
    sed/MonitorTMS.cpp)

  
  add_definitions(-DMODULE_PREFIX="${CMAKE_SHARED_MODULE_PREFIX}")
  add_definitions(-DMODULE_SUFFIX="${CMAKE_SHARED_MODULE_SUFFIX}")

  set(server_SRCS
    server/BatchServer.cpp
    server/SSHJobExec.cpp
    server/JobServer.cpp
    server/BatchFactory.cpp
    server/ListQueuesServer.cpp
    server/JobOutputServer.cpp
    server/ScriptGenConvertor.cpp
    server/Env.cpp
    server/WorkServer.cpp
    utils/SharedLibrary.cc
    )
 if(WIN32)
   set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_win32.cc)
 else()
   set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_posix.cc)
 endif()
 
  add_library(vishnu-tms-server ${server_SRCS})
  set_target_properties(vishnu-tms-server PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-tms-server vishnu-ums-server vishnu-ums-client)
  install(TARGETS vishnu-tms-server DESTINATION ${LIB_INSTALL_DIR})
  add_executable(tmssed ${sed_SRCS})
  target_link_libraries(tmssed
    ${Boost_LIBRARIES}
    ${DATABASE_LIBS}
    vishnu-core
    ${LIBCRYPT_LIB}
    vishnu-core-server
    vishnu-ums-server
    vishnu-tms-server
    vishnu-tms-client
    ${LDAP_LIBRARIES}
    emf4cpp-vishnu
    ${ZMQ_LIBRARIES}
    zmq_helper)

  if (torque)
    add_library(vishnu-tms-torque${VISHNU_BATCH_VERSION} ${TORQUESERVER})
    target_link_libraries(vishnu-tms-torque${VISHNU_BATCH_VERSION} ${TORQUE_LIB})
    install(TARGETS vishnu-tms-torque${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (ll)
    add_library(vishnu-tms-loadleveler${VISHNU_BATCH_VERSION} ${LOADLEVELERSERVER})
    target_link_libraries(vishnu-tms-loadleveler${VISHNU_BATCH_VERSION} ${LOADLEVELER_LIB})
    install(TARGETS vishnu-tms-loadleveler${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (slurm)
    add_library(vishnu-tms-slurm${VISHNU_BATCH_VERSION} ${SLURMSERVER})
    target_link_libraries(vishnu-tms-slurm${VISHNU_BATCH_VERSION} ${SLURM_LIB})
    install(TARGETS vishnu-tms-slurm${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (lsf)
    add_library(vishnu-tms-lsf${VISHNU_BATCH_VERSION} ${LSFSERVER})
    target_link_libraries(vishnu-tms-lsf${VISHNU_BATCH_VERSION} ${LSF_LIB})
    install(TARGETS vishnu-tms-lsf${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (sge)
    add_library(vishnu-tms-sge${VISHNU_BATCH_VERSION} ${SGESERVER})
    target_link_libraries(vishnu-tms-sge${VISHNU_BATCH_VERSION} ${SGE_LIB})
    install(TARGETS vishnu-tms-sge${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (pbs)
    add_library(vishnu-tms-pbspro${VISHNU_BATCH_VERSION} ${PBSPROSERVER})
    target_link_libraries(vishnu-tms-pbspro${VISHNU_BATCH_VERSION} ${PBSPRO_LIB})
    install(TARGETS vishnu-tms-pbspro${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if(deltacloud)
    add_library(vishnu-tms-deltacloud ${DELTACLOUDSERVER})
    target_link_libraries(vishnu-tms-deltacloud ${LIBDELTACLOUD_LIB})
    install(TARGETS vishnu-tms-deltacloud DESTINATION ${LIB_INSTALL_DIR})
  endif()
  
  # always compile posix server
  add_library(vishnu-tms-posix1.0 ${POSIXSERVER})
  target_link_libraries(vishnu-tms-posix1.0 ${POSIX_LIB} vishnu-core-server vishnu-tms-server)
  install(TARGETS vishnu-tms-posix1.0 DESTINATION ${LIB_INSTALL_DIR})

  install(TARGETS tmsSlave DESTINATION ${SBIN_INSTALL_DIR}) 
  install(TARGETS tmssed DESTINATION ${SBIN_INSTALL_DIR})

endif (COMPILE_SERVERS)

