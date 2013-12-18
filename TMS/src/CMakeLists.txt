######################################################################
#                         BATCH SCHEDULERS                           #
######################################################################
set(torque 0)
set(ll 0)
set(slurm 0)
set(lsf 0)
set(sge 0)	
set(pbs 0)
set(deltacloud 0)
set(posix 0)

if (COMPILE_TMS AND COMPILE_SERVERS)
  string (COMPARE EQUAL ${VISHNU_BATCH} "TORQUE" torque)
  string (COMPARE EQUAL ${VISHNU_BATCH} "LL" ll)
  string (COMPARE EQUAL ${VISHNU_BATCH} "SLURM" slurm)
  string (COMPARE EQUAL ${VISHNU_BATCH} "LSF" lsf)
  string (COMPARE EQUAL ${VISHNU_BATCH} "SGE" sge)
  string (COMPARE EQUAL ${VISHNU_BATCH} "PBS" pbs)
  string (COMPARE EQUAL ${VISHNU_BATCH} "DELTACLOUD" deltacloud)
  string (COMPARE EQUAL ${VISHNU_BATCH} "POSIX" posix)

  if (torque OR ll OR lsf OR slurm OR sge OR pbs OR deltacloud)
    set(posix 1)
  endif()
  
  if ( NOT (torque OR ll OR lsf OR slurm OR sge OR pbs OR deltacloud OR posix) )
    message(FATAL_ERROR "VISHNU_BATCH must be either TORQUE, LL, SLURM, LSF, SGE, PBS, DELTACLOUD or POSIX")
  endif()

  # posix does not need version, can be empty, posix set to 1 if another activated so can't use reverse condition
  if ( torque OR ll OR lsf OR slurm OR sge OR pbs )
    string(REPLACE "." "_" batch_version_tmp ${VISHNU_BATCH_VERSION})
  endif()

  if (torque)
    string (COMPARE EQUAL ${batch_version_tmp} "2_3" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid TORQUE version, supported are: 2.3")
    endif()
    
    find_package(TORQUE)
    if (TORQUE_FOUND)
    else (TORQUE_FOUND)
      message("TORQUE installation was not found. Define the TORQUE_DIR variable to continue.")
      message("     - You can define a TORQUE_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DTORQUE_DIR:PATH=/path/to/TORQUE")
      message("     - You can use the ccmake GUI")
      set(TORQUE_DIR "" cache path "TORQUE installation path")
    endif(TORQUE_FOUND)
  endif (torque)

  if (pbs)
    string (COMPARE EQUAL ${batch_version_tmp} "10_4" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid PBSPRO version, supported are: 10.4")
    endif()
    
    find_package(PBSPRO)
    if (PBSPRO_FOUND)
    else (PBSPRO_FOUND)
      message("PBSPRO installation was not found. Define the PBSPRO_DIR variable to continue.")
      message("     - You can define a PBSPRO_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DPBSPRO_DIR:PATH=/path/to/PBSPRO")
      message("     - You can use the ccmake GUI")
      set(PBSPRO_DIR "" cache path "PBSPRO installation path")
    endif(PBSPRO_FOUND)
  endif (pbs)

  if (ll)
    string (COMPARE EQUAL ${batch_version_tmp} "3_x" version)
    string (COMPARE EQUAL ${batch_version_tmp} "2_x" version2)
    set(batch_version_tmp "2_5")    
    if (version)
      set(batch_version_tmp "3_5")
    endif()
    if (NOT (version OR version2) )
      message (FATAL_ERROR "Invalid LoadLeveler version, supported are: 2.x, 3.x")
    endif()
    
    
    find_package(LOADLEVELER)
    if (LOADLEVELER_FOUND)
    else (LOADLEVELER_FOUND)
      message("LOADLEVELER installation was not found. Define the LOADLEVELER_DIR variable to continue.")
      message("     - You can define a LOADLEVELER_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLOADLEVELER_DIR:PATH=/path/to/LOADLEVELER")
      message("     - You can use the ccmake GUI")
      set(LOADLEVELER_DIR "" cache path "LOADLEVELER installation path")
    endif(LOADLEVELER_FOUND)
  endif (ll)

  if (slurm)
    if(${VISHNU_BATCH_VERSION} VERSION_GREATER "2.6")
       set(batch_version_not_supported true)
    else(${VISHNU_BATCH_VERSION} VERSION_GREATER "2.6")
       if(${VISHNU_BATCH_VERSION} VERSION_LESS "2.3")
         set(batch_version_tmp "2_2")
       else(${VISHNU_BATCH_VERSION} VERSION_LESS "2.3")
         set(batch_version_tmp "2_3")
       endif(${VISHNU_BATCH_VERSION} VERSION_LESS "2.3")
    endif(${VISHNU_BATCH_VERSION} VERSION_GREATER "2.6")

    if (batch_version_not_supported)
      message(FATAL_ERROR "Not sure to be compliant with your version of SLURM; versions officially tested are 2.x, x<=6")
    endif()

    # we add compilation variable definitions only on required files
    set_source_files_properties(
      ${SLURMSERVER}
      PROPERTIES COMPILE_FLAGS "-DHAVE_SLURM_${batch_version_tmp}"
      )    
    
    find_package(SLURM)
    if (SLURM_FOUND)
    else (SLURM_FOUND)
      message("SLURM installation was not found. Define the SLURM_DIR variable to continue.")
      message("     - You can define a SLURM_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DSLURM_DIR:PATH=/path/to/SLURM")
      message("     - You can use the ccmake GUI")
      set(SLURM_DIR "" cache path "SLURM installation path")
    endif(SLURM_FOUND)
  endif (slurm)

  if (lsf)
    string (COMPARE EQUAL ${batch_version_tmp} "7_0" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid LSF version, supported are: 7.0")
    endif()
    
    
    find_package(LSF)
    if (LSF_FOUND)
    else (LSF_FOUND)
      message("LSF installation was not found. Define the LSF_DIR variable to continue.")
      message("     - You can define a LSF_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLSF_DIR:PATH=/path/to/LSF")
      message("     - You can use the ccmake GUI")
      set(LSF_DIR "" cache path "LSF installation path")
    endif(LSF_FOUND)
  endif (lsf)

  if (sge)
    string (COMPARE EQUAL ${batch_version_tmp} "11" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid SGE version, supported are: 11")
    endif()


    find_package(SGE)
    if (SGE_FOUND)
    else (SGE_FOUND)
      message("SGE installation was not found. Define the SGE_DIR variable to continue.")
      message("     - You can define a SGE_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DSGE_DIR:PATH=/path/to/SGE")
      message("     - You can use the ccmake GUI")
      set(SGE_DIR "" cache path "SGE installation path")
    endif(SGE_FOUND)
  endif (sge)
  if (deltacloud)
    mark_as_advanced(VISHNU_BATCH_VERSION)
    find_package(LibDeltaCloud)
    if (LIBDELTACLOUD_FOUND)
    else (LIBDELTACLOUD_FOUND)
      message("LIBDELTACLOUD was not found. You may need to define LIBDELTACLOUD_DIR.")
      message("     - You can define a LIBDELTACLOUD_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLIBDELTACLOUD_DIR:PATH=/path/to/libdeltacloud")
      message("     - You can use the ccmake GUI")
      set(LIBDELTACLOUD_DIR "" cache path "LIBDELTACLOUD installation path")
    endif(LIBDELTACLOUD_FOUND)
  endif (deltacloud)

endif(COMPILE_TMS AND COMPILE_SERVERS)


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
  ${COMMUNICATION_INCLUDE_DIR}
  ${UMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${VISHNU_SOURCE_DIR}/TMS/src/sed
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/server/SGEConfig.hpp.in
  ${PROJECT_BINARY_DIR}/include/SGEConfig.hpp)

set(client_SRCS client/JobProxy.cpp
  client/JobOutputProxy.cpp
  client/WorkProxy.cpp
  ${UTILVISHNU_SOURCE_DIR}/tmsProxiesUtils.cpp)

set(cli_SRCS
  cli/displayer.cpp
  ${UTILVISHNU_SOURCE_DIR}/tmsUtils.cpp)

set(api_SRCS api/api_tms.cpp)
set(api_HEADERS api/api_tms.hpp)

if(COMPILE_TMS AND COMPILE_SERVERS AND NOT COMPILE_ONLY_LIBBATCH)
  add_library(vishnu-tms-client
    ${client_SRCS}
    ${cli_SRCS}
    ${api_SRCS})

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
endif(COMPILE_TMS AND COMPILE_SERVERS AND NOT COMPILE_ONLY_LIBBATCH)

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
  

 if(NOT COMPILE_ONLY_LIBBATCH)

   # define tmsSlave related source files
  set(slave_SRCS slave/slave.cpp ${UTILVISHNU_SOURCE_DIR}/tmsUtils.cpp)

  # define tmsSlave binary
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

  # set tmssed related source files
  set(sed_SRCS sed/tmssed.cpp
    sed/ServerTMS.cpp
    sed/internalApi.cpp
    sed/MonitorTMS.cpp
    ${COMMUNICATION_INCLUDE_DIR}/CommServer.cpp
    )
  
  add_definitions(-DMODULE_PREFIX="${CMAKE_SHARED_MODULE_PREFIX}")
  add_definitions(-DMODULE_SUFFIX="${CMAKE_SHARED_MODULE_SUFFIX}")

   # set tms library related source files
  set(server_SRCS
    server/BatchServer.cpp
    server/SSHJobExec.cpp
    server/JobServer.cpp
    server/BatchFactory.cpp
    server/ListQueuesServer.cpp
    server/JobOutputServer.cpp
    server/ScriptGenConvertor.cpp
    server/WorkServer.cpp
    utils/SharedLibrary.cc
    ${UTILVISHNU_SOURCE_DIR}/tmsUtils.cpp)

 if(WIN32)
   set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_win32.cc)
 else()
   set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_posix.cc)
 endif()
 
  # generate vishnu-tms-server library
  add_library(vishnu-tms-server ${server_SRCS})
  set_target_properties(vishnu-tms-server PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-tms-server vishnu-ums-server)
  install(TARGETS vishnu-tms-server DESTINATION ${LIB_INSTALL_DIR})

  # generate tmssed binary
  add_executable(tmssed ${sed_SRCS})
  target_link_libraries(tmssed
    ${Boost_LIBRARIES}
    ${DATABASE_LIBS}
    vishnu-core
    ${LIBCRYPT_LIB}
    vishnu-core-server
    vishnu-tms-server
    ${LDAP_LIBRARIES}
    emf4cpp-vishnu
    ${ZMQ_LIBRARIES}
    zmq_helper
    ${USED_BATCH_LIB})

  install(TARGETS tmsSlave DESTINATION ${SBIN_INSTALL_DIR})
  install(TARGETS tmssed DESTINATION ${SBIN_INSTALL_DIR})
 endif(NOT COMPILE_ONLY_LIBBATCH)

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

  # set the library for posix server => always compiled in the server
  add_library(vishnu-tms-posix ${POSIXSERVER})
  target_link_libraries(vishnu-tms-posix ${POSIX_LIB})
  install(TARGETS vishnu-tms-posix DESTINATION ${LIB_INSTALL_DIR})
endif(COMPILE_SERVERS)

