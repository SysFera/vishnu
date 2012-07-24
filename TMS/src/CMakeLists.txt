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
)

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


add_subdirectory(cli)

if (NOT CLIENT_ONLY)
  include(config.cmake)
  include_directories(server
    utils
    ${DATA_BASE_INCLUDE_DIR}
    ${CONFIG_SOURCE_DIR}
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
    ${ZMQ_LIBRARIES}
    zmq_helper
    )

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
    utils/SharedLibrary.cc)
 if(WIN32)
   set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_win32.cc)
 else()
   set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_posix.cc)
 endif()
 
  add_library(vishnu-tms-server ${server_SRCS})
  set_target_properties(vishnu-tms-server PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-tms-server ${USED_BATCH_LIB} vishnu-ums-server)
  install(TARGETS vishnu-tms-server DESTINATION ${LIB_INSTALL_DIR})
  add_executable(tmssed ${sed_SRCS})
  target_link_libraries(tmssed
    ${Boost_LIBRARIES}
    ${LOG_LIBRARY}
    ${DATABASE_LIBS}
    vishnu-core
    ${LIBCRYPT_LIB}
    vishnu-core-server
    vishnu-ums-server
    vishnu-tms-server
    ${LDAP_LIBRARIES}
    emf4cpp-vishnu
    ${USED_BATCH_LIB}
    ${ZMQ_LIBRARIES}
    zmq_helper)

  if (VISHNU_USE_TORQUE)
    add_library(vishnu-tms-torque ${TORQUESERVER_SRCS})
    target_link_libraries(vishnu-tms-torque ${TORQUE_LIB})
    install(TARGETS vishnu-tms-torque DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (VISHNU_USE_LOADLEVELER)
    add_library(vishnu-tms-loadleveler ${LOADLEVELERSERVER_SRCS})
    target_link_libraries(vishnu-tms-loadleveler ${LOADLEVELER_LIB})
    install(TARGETS vishnu-tms-loadleveler DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (VISHNU_USE_SLURM)
    add_library(vishnu-tms-slurm ${SLURMSERVER_SRCS})
    target_link_libraries(vishnu-tms-slurm ${SLURM_LIB})
    install(TARGETS vishnu-tms-slurm DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (VISHNU_USE_LSF)
    add_library(vishnu-tms-lsf ${LSFSERVER_SRCS})
    target_link_libraries(vishnu-tms-lsf ${LSF_LIB})
    install(TARGETS vishnu-tms-lsf DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if (VISHNU_USE_SGE)
    add_library(vishnu-tms-sge ${SGESERVER_SRCS})
    target_link_libraries(vishnu-tms-sge ${SGE_LIB})
    install(TARGETS vishnu-tms-sge DESTINATION ${LIB_INSTALL_DIR})
  endif()

  install(TARGETS tmsSlave DESTINATION ${SBIN_INSTALL_DIR}) 
  install(TARGETS tmssed DESTINATION ${SBIN_INSTALL_DIR})
endif (NOT CLIENT_ONLY)

