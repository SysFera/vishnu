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
  ${UMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${VISHNU_SOURCE_DIR}/TMS/src/sed
  ${LIBJANSSON_INCLUDE_DIR}
  ${COMMUNICATION_INCLUDE_DIR}
)

if (COMPILE_SERVER_TMS)
  string (COMPARE EQUAL ${VISHNU_BATCH} "TORQUE" torque)
  string (COMPARE EQUAL ${VISHNU_BATCH} "LL" ll)
  string (COMPARE EQUAL ${VISHNU_BATCH} "SLURM" slurm)
  string (COMPARE EQUAL ${VISHNU_BATCH} "LSF" lsf)
  string (COMPARE EQUAL ${VISHNU_BATCH} "SGE" sge)
  string (COMPARE EQUAL ${VISHNU_BATCH} "PBS" pbs)
  string (COMPARE EQUAL ${VISHNU_BATCH} "DELTACLOUD" deltacloud)
  string (COMPARE EQUAL ${VISHNU_BATCH} "OPENNEBULA" opennebula)
  string (COMPARE EQUAL ${VISHNU_BATCH} "POSIX" posix)

  if (torque OR ll OR lsf OR slurm OR sge OR pbs OR deltacloud OR opennebula)
    set(posix 1)
  endif()
  
  if ( NOT (torque OR ll OR lsf OR slurm OR sge OR pbs OR deltacloud OR posix OR opennebula) )
    message(FATAL_ERROR "VISHNU_BATCH must be either TORQUE, LL, SLURM, LSF, SGE, "
                         "PBS, DELTACLOUD, POSIX, or OPENNEBULA")
  endif()

  # posix does not need version, can be empty, posix set to 1 if another activated so can't use reverse condition
  if (torque OR ll OR lsf OR slurm OR sge OR pbs)
    string(REPLACE "." "_" batch_version_tmp ${VISHNU_BATCH_VERSION})
  endif()

  if (torque)
    string (COMPARE EQUAL ${batch_version_tmp} "2_3" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid TORQUE version, supported are: 2.3")
    endif()
    
    find_package(TORQUE)
    if (NOT TORQUE_FOUND)
      message("TORQUE installation was not found. Define the TORQUE_DIR variable to continue.")
      message("     - You can define a TORQUE_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DTORQUE_DIR=/path/to/TORQUE")
      message("     - You can use the ccmake GUI")
      set(TORQUE_DIR "" cache path "TORQUE installation path")
    endif(NOT TORQUE_FOUND)
  endif (torque)

  if (pbs)
    string (COMPARE EQUAL ${batch_version_tmp} "10_4" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid PBSPRO version, supported are: 10.4")
    endif()
    
    find_package(PBSPRO)
    if (NOT PBSPRO_FOUND)
      message("PBSPRO installation was not found. Define the PBSPRO_DIR variable to continue.")
      message("     - You can define a PBSPRO_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DPBSPRO_DIR=/path/to/PBSPRO")
      message("     - You can use the ccmake GUI")
      set(PBSPRO_DIR "" cache path "PBSPRO installation path")
    endif(NOT PBSPRO_FOUND)
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
    if(NOT LOADLEVELER_FOUND)
      message("LOADLEVELER installation was not found. Define the LOADLEVELER_DIR variable to continue.")
      message("     - You can define a LOADLEVELER_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLOADLEVELER_DIR=/path/to/LOADLEVELER")
      message("     - You can use the ccmake GUI")
      set(LOADLEVELER_DIR "" cache path "LOADLEVELER installation path")
    endif(NOT LOADLEVELER_FOUND)
  endif (ll)

  if(slurm)
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
      PROPERTIES COMPILE_FLAGS "-DHAVE_SLURM_${batch_version_tmp}")
    
    find_package(SLURM)
    if(NOT SLURM_FOUND)
      message("SLURM installation was not found. Define the SLURM_DIR variable to continue.")
      message("     - You can define a SLURM_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DSLURM_DIR=/path/to/SLURM")
      message("     - You can use the ccmake GUI")
      set(SLURM_DIR "" cache path "SLURM installation path")
    endif(NOT SLURM_FOUND)
  endif(slurm)

  if (lsf)
    string (COMPARE EQUAL ${batch_version_tmp} "7_0" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid LSF version, supported are: 7.0")
    endif()
    
    
    find_package(LSF)
    if (NOT LSF_FOUND)
      message("LSF installation was not found. Define the LSF_DIR variable to continue.")
      message("     - You can define a LSF_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLSF_DIR=/path/to/LSF")
      message("     - You can use the ccmake GUI")
      set(LSF_DIR "" cache path "LSF installation path")
    endif(NOT LSF_FOUND)
  endif (lsf)

  if (sge)
    string (COMPARE EQUAL ${batch_version_tmp} "11" version)
    if (NOT version)
      message (FATAL_ERROR "Invalid SGE version, supported are: 11")
    endif()

    find_package(SGE)
    if(NOT SGE_FOUND)
      message("SGE installation was not found. Define the SGE_DIR variable to continue.")
      message("     - You can define a SGE_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DSGE_DIR=/path/to/SGE")
      message("     - You can use the ccmake GUI")
      set(SGE_DIR "" cache path "SGE installation path")
    endif(NOT SGE_FOUND)
  endif(sge)
  if(deltacloud)
    mark_as_advanced(VISHNU_BATCH_VERSION)
    find_package(LibDeltaCloud)
    if (NOT LIBDELTACLOUD_FOUND)
      message("LIBDELTACLOUD was not found. You may need to define LIBDELTACLOUD_DIR.")
      message("     - You can define a LIBDELTACLOUD_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLIBDELTACLOUD_DIR=/path/to/libdeltacloud")
      message("     - You can use the ccmake GUI")
      set(LIBDELTACLOUD_DIR "" cache path "LIBDELTACLOUD installation path")
    endif(NOT LIBDELTACLOUD_FOUND)
  endif(deltacloud)

  if (opennebula)
    mark_as_advanced(VISHNU_BATCH_VERSION)
    find_package(LibXmlRpc)
    if(NOT LIBXMLRPC_FOUND)
      message("libxmlrpc was not found. You may need to define LIBXMLRPC_DIR.")
      message("     - You can define a LIBXMLRPC_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLIBXMLRPC_DIR=/path/to/libxmlrpc")
      message("     - You can use the ccmake GUI")
      set(LIBXMLRPC_DIR "" cache path "LIBXMLRPC_DIR installation path")
    endif(NOT LIBXMLRPC_FOUND)

    find_package(LibXercesCPP)
    find_package(LibXercesCPP)
    if(NOT LIBXERCESCPP_FOUND)
      message("libxmlrpc was not found. You may need to define LIBXERCESCPP_DIR.")
      message("     - You can define a LIBXMLRPC_DIR environment variable")
      message("     - You can pass it as argument to cmake:")
      message("       $ cmake <source root directory> -DLIBXERCESCPP_DIR=/path/to/libxerces-c++")
      message("     - You can use the ccmake GUI")
      set(LIBXERCESCPP_DIR "" cache path "LIBXERCESCPP_DIR installation path")
    endif(NOT LIBXERCESCPP_FOUND)
  endif(opennebula)

endif(COMPILE_SERVER_TMS)

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

## client CLI
if(COMPILE_CLIENT_TMS)
  add_subdirectory(cli)
  add_library(vishnu-tms-client
    ${client_SRCS}
    ${cli_SRCS}
    ${api_SRCS}
    ${utils_SRCS})

  target_link_libraries(vishnu-tms-client
    ${Boost_LIBRARIES}
    emf4cpp-vishnu
    vishnu-core
    vishnu-ums-client
    vishnu-fms-client
    ${LIBCRYPT_LIB}
    ${ZMQ_LIBRARIES}
    zmq_helper)

  install(TARGETS vishnu-tms-client DESTINATION ${LIB_INSTALL_DIR})
  set_target_properties(vishnu-tms-client PROPERTIES VERSION ${VISHNU_VERSION})
  install(FILES ${api_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})
endif()


if (COMPILE_SERVER_TMS)
  include(config.cmake)
  include_directories(server
    utils
    posix_parser
    ${DATA_BASE_INCLUDE_DIR}
    ${REGISTRY_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${FMS_SERVER_SOURCE_DIR}
    ${USED_BATCH_INCLUDE_DIR}
    ${XMS_SED_SOURCE_DIR}
    ${TESTVISHNU_SOURCE_DIR}/../third-party
    ${PROJECT_BINARY_DIR})
  

 if(NOT COMPILE_ONLY_LIBBATCH)
   # define tmsSlave related source files
  set(slave_SRCS slave/slave.cpp ${UTILVISHNU_SOURCE_DIR}/tmsUtils.cpp
    server/BatchFactory.cpp
    utils/SharedLibrary.cc
    ../../core/src/utils/utilPosix.cpp
    ../../communication/utils.cpp
    ${logger_SRCS})

 if(WIN32)
   set(slave_SRCS ${slave_SRCS} utils/SharedLibraryImpl_win32.cc)
 else()
   set(slave_SRCS ${slave_SRCS} utils/SharedLibraryImpl_posix.cc)
 endif()


  # define tmsSlave binary
  add_executable(tmsSlave ${slave_SRCS})
  target_link_libraries(tmsSlave
    ${Boost_LIBRARIES}
    ${CMAKE_DL_LIBS}
    vishnu-core
    emf4cpp-vishnu
    jansson
    ${USED_BATCH_LIB})

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
    ${UTILVISHNU_SOURCE_DIR}/tmsUtils.cpp
    ${logger_SRCS})

  if(WIN32)
    set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_win32.cc)
  else()
    set(server_SRCS ${server_SRCS} utils/SharedLibraryImpl_posix.cc)
  endif()

  # generate vishnu-tms-server library
  add_library(vishnu-tms-server ${server_SRCS})
  set_target_properties(vishnu-tms-server PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-tms-server zmq_helper)
  install(TARGETS vishnu-tms-server DESTINATION ${LIB_INSTALL_DIR})

  install(TARGETS tmsSlave DESTINATION ${SBIN_INSTALL_DIR})
 endif(NOT COMPILE_ONLY_LIBBATCH)

  if (torque)
    add_library(vishnu-tms-torque${VISHNU_BATCH_VERSION} ${TORQUESERVER})
    target_link_libraries(vishnu-tms-torque${VISHNU_BATCH_VERSION} ${TORQUE_LIB})
    install(TARGETS vishnu-tms-torque${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif(torque)
  if (ll)
    add_library(vishnu-tms-loadleveler${VISHNU_BATCH_VERSION} ${LOADLEVELERSERVER})
    target_link_libraries(vishnu-tms-loadleveler${VISHNU_BATCH_VERSION} ${LOADLEVELER_LIB})
    install(TARGETS vishnu-tms-loadleveler${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif(ll)
  if (slurm)
    add_library(vishnu-tms-slurm${VISHNU_BATCH_VERSION} ${SLURMSERVER})
    target_link_libraries(vishnu-tms-slurm${VISHNU_BATCH_VERSION} ${SLURM_LIB})
    install(TARGETS vishnu-tms-slurm${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif(slurm)
  if (lsf)
    add_library(vishnu-tms-lsf${VISHNU_BATCH_VERSION} ${LSFSERVER})
    target_link_libraries(vishnu-tms-lsf${VISHNU_BATCH_VERSION} ${LSF_LIB})
    install(TARGETS vishnu-tms-lsf${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif(lsf)
  if(sge)
    add_library(vishnu-tms-sge${VISHNU_BATCH_VERSION} ${SGESERVER})
    target_link_libraries(vishnu-tms-sge${VISHNU_BATCH_VERSION} ${SGE_LIB})
    install(TARGETS vishnu-tms-sge${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif()
  if(pbs)
    add_library(vishnu-tms-pbspro${VISHNU_BATCH_VERSION} ${PBSPROSERVER})
    target_link_libraries(vishnu-tms-pbspro${VISHNU_BATCH_VERSION} ${PBSPRO_LIB})
    install(TARGETS vishnu-tms-pbspro${VISHNU_BATCH_VERSION} DESTINATION ${LIB_INSTALL_DIR})
  endif(pbs)
  if(deltacloud)
    add_library(vishnu-tms-deltacloud ${DELTACLOUDSERVER})
    target_link_libraries(vishnu-tms-deltacloud ${LIBDELTACLOUD_LIB})
    install(TARGETS vishnu-tms-deltacloud DESTINATION ${LIB_INSTALL_DIR})
  endif(deltacloud)
  if(opennebula)
    add_library(vishnu-tms-opennebula ${OPENNEBULA_UTILS} ${OPENNEBULASERVER} ${logger_SRCS})
    target_link_libraries(vishnu-tms-opennebula ${LIBXMLRPC_LIB} ${LIBXERCESCPP_LIB})
    install(TARGETS vishnu-tms-opennebula DESTINATION ${LIB_INSTALL_DIR})
  endif(opennebula)

  # set the library for posix server => always compiled in the server
  add_library(vishnu-tms-posix ${POSIXSERVER})
  target_link_libraries(vishnu-tms-posix ${POSIX_LIB})
  install(TARGETS vishnu-tms-posix DESTINATION ${LIB_INSTALL_DIR})
endif(COMPILE_SERVER_TMS)

