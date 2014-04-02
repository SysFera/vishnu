# Root CMakeLists.txt file
#
# Project: VISHNU
# Author: E. PAMBA CAPO-CHICHI
# Date: 4th February 2011

######################################################################
#                         GLOBAL PARAMETER                           #
######################################################################
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

set(VISHNU_VERSION "6.4.4" CACHE STRING " the version of the source ")
set(VISHNU_VERSION_COMMENTS "" CACHE STRING " comments about the current version ")



######################################################################
#                             OPTIONS                                #
######################################################################
# Options set to ON by default
option(BUILD_SHARED_LIBS "disable shared libraries usage" ON)

find_package(POSTGRESQL)
if (POSTGRESQL_FOUND)
  option(ENABLE_POSTGRESQL "compile with postgresql support" ON)
else (POSTGRESQL_FOUND)
  option(ENABLE_POSTGRESQL "compile with postgresql support" OFF)
endif(POSTGRESQL_FOUND)

find_package(MYSQL)
if (MYSQL_FOUND)
  option(ENABLE_MYSQL "compile with mysql support" ON)
else (MYSQL_FOUND)
  option(ENABLE_MYSQL "compile with mysql support" OFF)
endif(MYSQL_FOUND)


# COMPILE_UMS option set to ON by default
option(COMPILE_UMS "Enable both UMS client and server" ON)

# Options set to OFF by default
option(COMPILE_CLIENT "compile client libraries and CLI binaries" OFF)
option(COMPILE_CLIENT_FMS "Enable FMS client libraries and CLI binaries" OFF)
option(COMPILE_CLIENT_TMS "Enable TMS client libraries and CLI binaries" OFF)
option(COMPILE_CLIENT_UMS "Enable UMS client libraries and CLI binaries" OFF)
option(COMPILE_SERVER "compile the servers" OFF)
option(COMPILE_DISPATCHER "Enable the Dispacther" OFF)
option(COMPILE_SERVER_FMS "Enable FMS server libraries and binaries" OFF)
option(COMPILE_SERVER_TMS "Enable TMS server libraries and binaries" OFF)
option(COMPILE_SERVER_UMS "Enable UMS server libraries and binaries" OFF)
option(ENABLE_PYTHON "compile the python api (default: 2.x)" OFF)
option(ENABLE_PYTHON3 "target python3 instead of python2" OFF)
option(ENABLE_JAVA "compile the java api" OFF)
option(ENABLE_SWIG "use swig to re-generate code for the wrappers" OFF)
option(ENABLE_MYSQL "compile with the mysql support." OFF)
option(COMPILE_TMS "Enable both TMS client and server" OFF)
option(COMPILE_FMS "Enable both FMS client and server" OFF)
option(INSTALL_SYSV "install systemV init scripts" OFF)
option(INSTALL_SYSTEMD "install systemd services" OFF)
option(COMPILE_ONLY_LIBBATCH "compile only the dynamic library for a given batch scheduler type" OFF)

if(COMPILE_FMS)
 set(COMPILE_CLIENT_FMS ON CACHE BOOL "FMS Client actived" FORCE)
 set(COMPILE_SERVER_FMS ON CACHE BOOL "FMS Server actived" FORCE)
endif(COMPILE_FMS)

if(COMPILE_UMS)
 set(COMPILE_CLIENT_UMS ON CACHE BOOL "UMS Client actived" FORCE)
 set(COMPILE_SERVER_UMS ON CACHE BOOL "UMS Server actived" FORCE)
endif(COMPILE_UMS)

if(COMPILE_TMS)
 set(COMPILE_CLIENT_TMS ON CACHE BOOL "TMS Client actived" FORCE)
 set(COMPILE_SERVER_TMS ON CACHE BOOL "TMS Server actived" FORCE)
endif(COMPILE_TMS)

if(COMPILE_CLIENT)
  set(COMPILE_CLIENT_UMS ON CACHE BOOL "UMS Client actived" FORCE)
  set(COMPILE_CLIENT_TMS ON CACHE BOOL "TMS Client actived" FORCE)
  set(COMPILE_CLIENT_FMS ON CACHE BOOL "FMS Client actived" FORCE)
endif(COMPILE_CLIENT)

if(COMPILE_SERVER)
  set(COMPILE_DISPATCHER ON CACHE BOOL "Dispatcher actived" FORCE)
  set(COMPILE_SERVER_UMS ON CACHE BOOL "UMS Server actived" FORCE)
  set(COMPILE_SERVER_TMS ON CACHE BOOL "TMS Server actived" FORCE)
  set(COMPILE_SERVER_FMS ON CACHE BOOL "FMS Server actived" FORCE)
endif(COMPILE_SERVER)

if(COMPILE_CLIENT_FMS)
  set(COMPILE_CLIENT_UMS ON CACHE BOOL "UMS client actived" FORCE)
endif()

if(COMPILE_CLIENT_TMS)
  set(COMPILE_CLIENT_UMS ON CACHE BOOL "UMS client actived" FORCE)
  set(COMPILE_CLIENT_FMS ON CACHE BOOL "FMS client actived" FORCE)
endif()

if(COMPILE_ONLY_LIBBATCH)
  set(COMPILE_SERVER ON)
  set(COMPILE_SERVER_TMS ON)
  set(COMPILE_CLIENT_TMS OFF)
  set(COMPILE_CLIENT_UMS OFF)
  set(COMPILE_SERVER_UMS OFF)
  set(COMPILE_CLIENT_FMS OFF)
  set(COMPILE_CLIENT_TMS OFF)
  set(COMPILE_CLIENT_FMS OFF)
  set(COMPILE_SERVER_FMS OFF)
  set(COMPILE_CLIENT_UMS OFF)
  set(COMPILE_CLIENT OFF)
endif(COMPILE_ONLY_LIBBATCH)

set(VISHNU_BATCH "POSIX" CACHE path "Batch to use (TORQUE, LL, SLURM, LSF, SGE, PBS, POSIX, DELTACLOUD)")
set(VISHNU_BATCH_VERSION "1" CACHE path "Version of the batch scheduler")

if (COMPILE_UMS AND COMPILE_TMS AND COMPILE_FMS)
  set(COMPILE_ALL_MODULES 1)
endif (COMPILE_UMS AND COMPILE_TMS AND COMPILE_FMS)

option(ENABLE_LDAP   "Build VISHNU with LDAP support")
option(BUILD_TESTING "Enable unit testing" OFF)


######################################################################
#                             PACKAGES                               #
######################################################################
find_package(ZMQ)
find_package(OpenSSL)
find_package(LibJansson REQUIRED)

find_package(LIBCRYPT)
if(NOT LIBCRYPT_FOUND)
  message("LIBCRYPT installation was not found. Define the LIBCRYPT_DIR variable to continue.")
  message("     - You can define a LIBCRYPT_DIR environment variable")
  message("     - You can pass it as argument to cmake:")
  message("       $ cmake <source root directory> -DLIBCRYPT_DIR:PATH=/path/to/LIBCRYPT")
  message("     - You can use the ccmake GUI")
  set(LIBCRYPT_DIR "" cache path "LIBCRYPT installation path")
endif(NOT LIBCRYPT_FOUND)

if (BUILD_TESTING)
  find_package(Boost 1.46 REQUIRED program_options date_time thread filesystem system serialization random regex unit_test_framework)
else (BUILD_TESTING)
  find_package(Boost 1.46 REQUIRED program_options date_time thread filesystem system serialization random regex)
endif(BUILD_TESTING)

find_package(Threads)
if(THREADS_FOUND)
  set(THREADS_LIB ${CMAKE_THREAD_LIBS_INIT})
endif()

set(Boost_LIBRARIES ${Boost_LIBRARIES} ${THREADS_LIB})


if(APPLE)
  add_definitions("-DBSD_LIKE_SYSTEM")  
endif()


######################################################################
#                                LDAP                                #
######################################################################
if (ENABLE_LDAP)
  find_package(LDAP)
  if (LDAP_FOUND)
    find_package(OpenSSL)
    if (OPENSSL_FOUND)
      set(LDAP_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/authenticator/ldap)
    else (OPENSSL_FOUND)
      message("Openssl is not found.")
    endif(OPENSSL_FOUND)
  else (LDAP_FOUND)
    message("LDAP is not found. You must install OpenLDAP: slapd 2.4.21.")
  endif(LDAP_FOUND)
endif(ENABLE_LDAP)


######################################################################
#                             DATABASES                              #
######################################################################
if (ENABLE_POSTGRESQL)
  find_package(POSTGRESQL)
  if (POSTGRESQL_FOUND)
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

if (COMPILE_SERVER AND NOT ENABLE_POSTGRESQL AND NOT ENABLE_MYSQL)
   message(FATAL_ERROR  "You MUST set either ENABLE_POSTGRESQL or ENABLE_MYSQL")
endif()


######################################################################
#                           COMPILATION                              #
######################################################################
# communication
set(COMMUNICATION_INCLUDE_DIR
  ${VISHNU_SOURCE_DIR}/communication
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
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/contrib/vishnu-sample.cfg.in
    ${PROJECT_BINARY_DIR}/vishnu-sample.cfg)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/contrib/supervisord-sample.cfg.in
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


if (BUILD_TESTING)
  # Common directories for tests
  set(TEST_BIN_PATH "${PROJECT_BINARY_DIR}/bin" CACHE STRING "Contains binaries for tests")
  mark_as_advanced(TEST_BIN_PATH)

  add_subdirectory(core/test)
endif (BUILD_TESTING)

add_subdirectory(communication)

if(COMPILE_CLIENT_UMS OR COMPILE_SERVER_UMS)
  add_subdirectory(UMS/src)
  add_subdirectory(UMS/doc/man)
  if (BUILD_TESTING)
    add_subdirectory(UMS/test/src)
    set(FMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/test/src)
    set(UMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/test/src)
    set(TMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_CLIENT_UMS OR COMPILE_SERVER_UMS)

if(COMPILE_CLIENT_FMS OR COMPILE_SERVER_FMS)
  add_subdirectory(FMS/src)
  add_subdirectory(FMS/doc/man)
  if (BUILD_TESTING)
      add_subdirectory(FMS/test/src)
      set(FMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/FMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_CLIENT_FMS OR COMPILE_SERVER_FMS)

if(COMPILE_CLIENT_TMS OR COMPILE_SERVER_TMS)
  add_subdirectory(TMS/src)
  add_subdirectory(TMS/doc/man)
  if (BUILD_TESTING)
    add_subdirectory(TMS/test/src)
    set(TMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_CLIENT_TMS OR COMPILE_SERVER_TMS)


## KEEP AFTER VARIABLE DECLARATION
# set dependencies
if (ENABLE_PYTHON3)
  set(ENABLE_PYTHON ON CACHE BOOL "Force Python compilation" FORCE)
endif()
if (ENABLE_PYTHON OR ENABLE_JAVA)
  if (NOT ENABLE_SWIG)
    set (COMPILE_CLIENT_UMS ON CACHE BOOL "UMS client actived" FORCE)
    set (COMPILE_CLIENT_FMS ON CACHE BOOL "We need to build FMS" FORCE)
    set (COMPILE_CLIENT_TMS ON CACHE BOOL "TMS client actived" FORCE)
  endif()
  add_subdirectory(swigAPI)
endif(ENABLE_PYTHON OR ENABLE_JAVA)


# module that adds an uninstall target
include(CMakeUninstall)
add_subdirectory(contrib)
