# Root CMakeLists.txt file
#
# Project: VISHNU
# Author: E. PAMBA CAPO-CHICHI
# Date: 4th February 2011

cmake_minimum_required(VERSION 2.8.5)
project(VISHNU)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Cmake)
# set default build type to debug
## set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

set( VISHNU_VERSION "3.0.0" CACHE STRING " the version of the source ")
set( VISHNU_VERSION_COMMENTS "beta1" CACHE STRING " comments about the current version ")

# Options set to ON by default
option(BUILD_SHARED_LIBS "disable shared libraries usage" ON)

find_package(POSTGRESQL)
if (POSTGRESQL_FOUND)
  option(ENABLE_POSTGRESQL "compile with postgresql support" ON)
else (POSTGRESQL_FOUND)
  option(ENABLE_POSTGRESQL "compile with postgresql support" OFF)
  find_package(MYSQL)
  if (MYSQL_FOUND)
    option(ENABLE_MYSQL "compile with mysql support" ON)
  else (MYSQL_FOUND)
    option(ENABLE_MYSQL "compile with mysql support" OFF)
  endif(MYSQL_FOUND)
endif(POSTGRESQL_FOUND)
# COMPILE_UMS option set to ON by default
option(COMPILE_UMS "compile the module UMS" ON)
# INSTALL_SYSV option set to OFF by default
option(INSTALL_SYSV "install systemV init scripts" OFF)
# INSTALL_SYSTEMD option set to OFF by default
option(INSTALL_SYSTEMD "install systemd services" OFF)

# Options set to OFF by default
option(COMPILE_CLIENT_CLI "compile the client command line interface")
option(COMPILE_SERVERS "compile the servers")
option(ENABLE_PYTHON "compile the python api")
option(ENABLE_JAVA "compile the java api")
option(ENABLE_SWIG "use swig to re-generate code for the wrappers")
option(ENABLE_MYSQL "compile with the mysql support.")
option(COMPILE_TMS "compile the module TMS")
option(COMPILE_FMS "compile the module FMS")
option(COMPILE_IMS "compile the module IMS")

set(VISHNU_BATCH "TORQUE" CACHE path "Batch to use (TORQUE, LL, SLURM, LSF, SGE, PBS, POSIX)")
set(VISHNU_BATCH_VERSION "2.3" CACHE path "Version of the batch scheduler")

#option(VISHNU_USE_LOADLEVELER   "Build VISHNU with LoadLeveler support...")
#option(VISHNU_USE_TORQUE   "Build VISHNU with Torque support...")
#option(VISHNU_USE_SLURM   "Build VISHNU with SLURM support...")
#option(VISHNU_USE_LSF   "Build VISHNU with LSF support...")
#option(VISHNU_USE_SGE     "Build VISHNU with SGE support...")

option(VISHNU_USE_LDAP   "Build VISHNU with LDAP support...")

if (COMPILE_UMS AND COMPILE_TMS AND COMPILE_FMS AND COMPILE_IMS)
  set(COMPILE_ALL_MODULES 1)
endif (COMPILE_UMS AND COMPILE_TMS AND COMPILE_FMS AND COMPILE_IMS)

# FIXME: cache is not updated when CLIENT_ONLY is enabled and BUILD_TESTING previously set to ON
# include(CMakeDependentOption)
# cmake_dependent_option(BUILD_TESTING "Enable unit testing" OFF "NOT CLIENT_ONLY" OFF)
option(DEBUG_TESTS "Provide debug info during tests execution" OFF)

find_package(ZMQ)

find_package(LIBCRYPT)
if (LIBCRYPT_FOUND)
else (LIBCRYPT_FOUND)
  message("LIBCRYPT installation was not found. Define the LIBCRYPT_DIR variable to continue.")
  message("     - You can define a LIBCRYPT_DIR environment variable")
  message("     - You can pass it as argument to cmake:")
  message("       $ cmake <source root directory> -DLIBCRYPT_DIR:PATH=/path/to/LIBCRYPT")
  message("     - You can use the ccmake GUI")
  set(LIBCRYPT_DIR "" cache path "LIBCRYPT installation path")
endif(LIBCRYPT_FOUND)

if (ENABLE_POSTGRESQL)
  find_package(POSTGRESQL)
  if (POSTGRESQL_FOUND)
    add_definitions(-DUSE_POSTGRES)
    set(DATABASE_LIBS ${DATABASE_LIBS} ${POSTGRESQL_LIB})
    set(DATABASE_INCLUDE_DIR ${DATABASE_INCLUDE_DIR} ${POSTGRESQL_INCLUDE_DIR})
  else (POSTGRESQL_FOUND)
    message("POSTGRESQL installation was not found. Define the POSTGRESQL_DIR variable to continue.")
    message("     - You can define a POSTGRESQL_DIR environment variable")
    message("     - You can pass it as argument to cmake:")
    message("       $ cmake <source root directory> -DPOSTGRESQL_DIR:PATH=/path/to/POSTGRESQL")
    message("     - You can use the ccmake GUI")
    set(POSTGRESQL_DIR "" cache path "POSTGRESQL installation path")
  endif(POSTGRESQL_FOUND)
endif (ENABLE_POSTGRESQL)

if (ENABLE_ORACLE)
  find_package(ORACLE)
  if (ORACLE_FOUND)
  else (ORACLE_FOUND)
    message("ORACLE installation was not found. Define the ORACLE_DIR variable to continue.")
    message("     - You can define a ORACLE_DIR environment variable")
    message("     - You can pass it as argument to cmake:")
    message("       $ cmake <source root directory> -DORACLE_DIR:PATH=/path/to/ORACLE")
    message("     - You can use the ccmake GUI")
    set(ORACLE_DIR "" cache path "ORACLE installation path")
  endif(ORACLE_FOUND)
endif (ENABLE_ORACLE)
SET (ORACLE_FOUND false)

if (ENABLE_MYSQL)
  add_definitions(-DUSE_MYSQL)
  find_package(MYSQL)
  if (MYSQL_FOUND)
    set(DATABASE_LIBS ${DATABASE_LIBS} ${MYSQL_LIB})
    set(DATABASE_INCLUDE_DIR ${DATABASE_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR})
  else (MYSQL_FOUND)
    message("MYSQL installation was not found. Define the MYSQL_DIR variable to continue.")
    message("     - You can define a MYSQL_DIR environment variable")
    message("     - You can pass it as argument to cmake:")
    message("       $ cmake <source root directory> -DMYSQL_DIR:PATH=/path/to/MYSQL")
    message("     - You can use the ccmake GUI")
    set(MYSQL_DIR "" cache path "MYSQL installation path")
  endif(MYSQL_FOUND)
endif (ENABLE_MYSQL)

if (COMPILE_SERVERS AND NOT ENABLE_POSTGRESQL  AND NOT ENABLE_MYSQL)
   message(FATAL_ERROR  "You MUST set either ENABLE_POSTGRESQL or ENABLE_MYSQL")
endif()

set(torque 0)
set(ll 0)
set(slurm 0)
set(lsf 0)
set(sge 0)	
set(pbs 0)
set(deltacloud 0)
set(posix 0)

if (COMPILE_TMS AND COMPILE_SERVERS) # AND NOT VISHNU_USE_LOADLEVELER AND NOT VISHNU_USE_TORQUE AND NOT VISHNU_USE_SLURM AND NOT VISHNU_USE_LSF AND NOT VISHNU_USE_SGE)
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
  set (POSIX_TEST_LIB vishnu-tms-posix1.0)
  
  if ( NOT (torque OR ll OR lsf OR slurm OR sge OR pbs OR deltacloud OR posix) )
    message(FATAL_ERROR "VISHNU_BATCH must be either TORQUE, LL, SLURM, LSF, SGE, PBS, DELTACLOUD or POSIX")
  endif()
endif()

if (COMPILE_TMS)
  add_definitions(-DCOMPILE_TMS)
  set (COMPILE_FMS 1)

  # posix does not need version, can be empty, posix set to 1 if another activated so can't use reverse condition
  if ( torque OR ll OR lsf OR slurm OR sge OR pbs )
    string(REPLACE "." "_" batch_version_tmp ${VISHNU_BATCH_VERSION})
  endif()

endif(COMPILE_TMS)


if (torque)
  string (COMPARE EQUAL ${batch_version_tmp} "2_3" version)
  if (NOT version)
    message (FATAL_ERROR "Invalid TORQUE version, supported are:
      2.3")
  endif()
  add_definitions(-DHAVE_TORQUE_${batch_version_tmp})

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
    message (FATAL_ERROR "Invalid PBSPRO version, supported are:
      10.4")
  endif()
  add_definitions(-DHAVE_PBSPRO_${batch_version_tmp})

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
  if (version2)
    set(batch_version_tmp "3_5")
  endif()
  if (NOT (version OR version2) )
    message (FATAL_ERROR "Invalid LoadLeveler version, supported are:
      3.x
      2.x")
  endif()

  add_definitions(-DHAVE_LOADLEVELER_${batch_version_tmp})

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
  string (COMPARE EQUAL ${batch_version_tmp} "2_2" version)
  string (COMPARE EQUAL ${batch_version_tmp} "2_3" version2)
  string (COMPARE EQUAL ${batch_version_tmp} "2_4" version3)
# Slurm 2.4 compile with 2.3 implementation
  if (version3)
    set(batch_version_tmp "2_3")
  endif()
  if (NOT (version OR version2 OR version3) )
    message (FATAL_ERROR "Invalid SLURM version, supported are:
      2.2 or 2.3 or 2.4")
  endif()

  add_definitions(-DHAVE_SLURM_${batch_version_tmp})

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
    message (FATAL_ERROR "Invalid LSF version, supported are:
      7.0")
  endif()

  add_definitions(-DHAVE_LSF_${batch_version_tmp})

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
    message (FATAL_ERROR "Invalid SGE version, supported are:
      11")
  endif()

  add_definitions(-DHAVE_SGE_${batch_version_tmp})

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
  add_definitions(-DHAVE_DELTACLOUD)
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


if (posix)
  add_definitions(-DHAVE_TMSPOSIX)
endif(posix)

if (VISHNU_USE_LDAP AND COMPILE_SERVERS)
  #To check if the module UMS is activated
  if (COMPILE_UMS)
    find_package(LDAP)
    if (LDAP_FOUND)
      find_package(OpenSSL)
      if (OPENSSL_FOUND)
        set(LDAP_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/authenticator/ldap)
        add_definitions(-DUSE_LDAP)
      else (OPENSSL_FOUND)
        message("Openssl is not found.")
      endif(OPENSSL_FOUND)
    else (LDAP_FOUND)
      message("LDAP is not found. You must install OpenLDAP: slapd 2.4.21.")
    endif(LDAP_FOUND)
  else (COMPILE_UMS)
    message("To use LDAP, the UMS module must be activated.")
  endif (COMPILE_UMS)
endif(VISHNU_USE_LDAP AND COMPILE_SERVERS)


find_package(Boost 1.46
  REQUIRED program_options date_time thread filesystem system unit_test_framework serialization random regex)

find_package(Threads)
if(THREADS_FOUND)
  set(THREADS_LIB ${CMAKE_THREAD_LIBS_INIT})
endif()

set(Boost_LIBRARIES ${Boost_LIBRARIES} ${THREADS_LIB})

add_definitions("-DBOOST_FILESYSTEM_VERSION=3")

if(APPLE)
  add_definitions("-DBSD_LIKE_SYSTEM")  
endif()


# mock
set(MOCK_INCLUDE_DIR
  ${VISHNU_SOURCE_DIR}/mockZMQ
)

#Core
set(EMF4CPP_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/deps/emf4cpp)
set(VISHNU_EXCEPTION_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/exception)
set(DATA_BASE_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/database)
set(REGISTRY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/registry)
set(UTILVISHNU_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/utils)
set(CONFIG_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/config)
set(EMF_DATA_DIR ${VISHNU_SOURCE_DIR}/core/src/emfdata)
set(AUTHENTICATOR_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/authenticator)
set(VERSION_MANAGER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/version)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vishnu_version.hpp.in
    ${PROJECT_BINARY_DIR}/vishnu_version.hpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core/examples/confs/vishnu-sample.cfg.in
    ${PROJECT_BINARY_DIR}/vishnu-sample.cfg)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core/examples/confs/supervisord-sample.cfg.in
    ${PROJECT_BINARY_DIR}/supervisord-sample.cfg)
#UMS
set(UMS_EMF_DATA_DIR ${EMF_DATA_DIR}/UMS_Data)
set(UMS_CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/client)
set(UMS_SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/server)
set(UMS_API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/api)
set(UMS_SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/sed)

#TMS
set(TMS_EMF_DATA_DIR ${EMF_DATA_DIR}/TMS_Data)
set(TMS_CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/client)
set(TMS_SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/server)
set(TMS_API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/api)
set(TMS_CLI_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/cli)
set(TMS_SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/sed)
set(TMS_UTILS_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils/)
set(UTILS_TORQUE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils_torque/)
set(UTILS_PBSPRO_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils_pbs/)
set(COMMON_DELTACLOUD_DIR ${VISHNU_SOURCE_DIR}/TMS/src/deltacloudcommon/)
set(SLURM_PARSER_DIR ${VISHNU_SOURCE_DIR}/TMS/src/slurm_parser/)
set(LSF_PARSER_DIR ${VISHNU_SOURCE_DIR}/TMS/src/lsf_parser/)

#IMS
set(IMS_EMF_DATA_DIR ${EMF_DATA_DIR}/IMS_Data)
set(IMS_CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/client)
set(IMS_SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/server)
set(IMS_API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/api)
set(IMS_SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/sed)
set(IMS_UTIL_DIR ${VISHNU_SOURCE_DIR}/IMS/src/utils)

#FMS
set(FMS_EMF_DATA_DIR ${EMF_DATA_DIR}/FMS_Data)
set(FMS_CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/src/client)
set(FMS_SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/src/server)
set(FMS_API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/src/api)
set(FMS_SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/src/sed)
set(FMS_UTIL_DIR ${VISHNU_SOURCE_DIR}/FMS/src/utils)



# gather executables and libraries under one roof
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(BIN_DIR ${EXECUTABLE_OUTPUT_PATH})

# ---------------- SET THE LINKER SEARCH PATH (RPATH) ----------------------
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON
    CACHE BOOL "Whether to set an rpath for dynamic libraries.")
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
    CACHE STRING "Rpath set at install stage." FORCE)

# for testing
set(TESTVISHNU_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/test/src)

# installation customization hooks
# manage lib/lib64 install issue
set(LIB_SUFFIX "" CACHE STRING "Define suffix of lib directory name (32/64)")
set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}"
  CACHE INTERNAL "Directory where libraries will be installed")
set(BIN_INSTALL_DIR "bin"
  CACHE INTERNAL "Directory where binaries will be installed")
set(SBIN_INSTALL_DIR "sbin"
  CACHE INTERNAL "Directory where privilegied binaries will be installed")
set(INCLUDE_INSTALL_DIR "include"
  CACHE INTERNAL "Directory where includes will be installed")
set(ETC_INSTALL_DIR "etc"
  CACHE INTERNAL "Directory where configuration files will be installed")
set(MAN_INSTALL_DIR "share/man"
  CACHE INTERNAL "Directory where man pages will be installed")
set(CMAKE_MOD_INSTALL_DIR "share/cmake/Modules"
  CACHE INTERNAL "Directory where cmake modules will be installed")

if(BUILD_TESTING)
  include(UnitTest)
  unit_test_setup()
endif()

add_subdirectory(core/src)
add_subdirectory(core/deps)

if (DEBUG_TESTS)
  add_definitions(-DDEBUG_TESTS)
endif (DEBUG_TESTS)

if (BUILD_TESTING)
  add_definitions(-DBUILD_TESTING)

  # Common directories for tests
  set(TEST_BIN_PATH "${PROJECT_BINARY_DIR}/bin" CACHE STRING "Contains binaries for tests")
  mark_as_advanced(TEST_BIN_PATH)

  add_subdirectory(core/test)
endif (BUILD_TESTING)

# It's required to enable all the modules when SWIG is enabled. Otherwise you'll have linking problems
# To avoid dealing with IMS dependencies when you only need API, you may need to set CLIENT_ONLY at ON
if (ENABLE_PYTHON OR ENABLE_JAVA)
  set (COMPILE_UMS 1 )
  set (COMPILE_FMS 1 )
  set (COMPILE_TMS 1 )
  set (COMPILE_IMS 1 )
  add_subdirectory(swigAPI)
endif(ENABLE_PYTHON OR ENABLE_JAVA)

add_subdirectory(mockZMQ)


if(COMPILE_UMS)
  add_subdirectory(UMS/src)
  add_subdirectory(UMS/doc/man)
  add_definitions(-DCOMPILE_UMS)
  if (BUILD_TESTING)
    add_subdirectory(UMS/test/src)
    set(FMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/test/src)
    set(UMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/test/src)
    set(TMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_UMS)

if(COMPILE_FMS)
  add_subdirectory(FMS/src)
  add_subdirectory(FMS/doc/man)
  add_definitions(-DCOMPILE_FMS)
  if (BUILD_TESTING)
      add_subdirectory(FMS/test/src)
      set(FMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_FMS)

if(COMPILE_TMS)
  add_subdirectory(TMS/src)
  add_subdirectory(TMS/doc/man)
  add_definitions(-DCOMPILE_TMS)
  if (BUILD_TESTING)
    add_subdirectory(TMS/test/src)
    set(TMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_TMS)

if(COMPILE_IMS)
  if (COMPILE_SERVERS)
    find_package(XMLRPC)
    find_package(Sigar REQUIRED)
  endif()
  add_definitions(-DCOMPILE_IMS)
  add_subdirectory(IMS/src)
  add_subdirectory(IMS/doc/man)
  if (BUILD_TESTING)
    add_subdirectory(IMS/test/src)
    set(IMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_IMS)



# module that adds an uninstall target
include(CMakeUninstall)
add_subdirectory(contrib)
