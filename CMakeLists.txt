# Root CMakeLists.txt file
#
# Project: VISHNU
# Author: E. PAMBA CAPO-CHICHI
# Date: 4th February 2011

cmake_minimum_required(VERSION 2.6)
project(VISHNU)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Cmake)
set (MANDIR /usr/share/man CACHE string "the man path")

set( VISHNU_VERSION "1.1.0" CACHE STRING " the version of the source ")

option(BUILD_SHARED_LIBS ON "disable shared libraries usage")
option(CLIENT_ONLY OFF "only compile the client side")
option(ENABLE_PYTHON OFF "compile the python api")
option(ENABLE_JAVA OFF "compile the java api")
option(ENABLE_ORACLE OFF "compile with the oracle support. Deprecated")
option(ENABLE_MYSQL OFF "compile with the mysql support.")
option(COMPILE_UMS OFF "compile the module UMS")
option(COMPILE_TMS OFF "compile the module TMS")
option(COMPILE_IMS OFF "compile the module IMS")
option(VISHNU_USE_TORQUE   "Build VISHNU with Torque support..." OFF)
option(VISHNU_USE_LOADLEVELER   "Build VISHNU with LoadLeveler support..." OFF)
# FIXME: cache is not updated when CLIENT_ONLY is enabled and BUILD_TESTING previously set to ON
include(CMakeDependentOption)
cmake_dependent_option(BUILD_TESTING "Enable unit testing" OFF "NOT CLIENT_ONLY" OFF)
option(DEBUG_TESTS "Provide debug info during tests execution" OFF)

find_package(DIET)
if (DIET_FOUND)
else (DIET_FOUND)
  message("DIET installation was not found. Define the DIET_DIR variable to continue.")
  message("	- You can define a DIET_DIR environment variable")
  message("	- You can pass it as argument to cmake:")
  message("	  $ cmake <source root directory> -DDIET_DIR:PATH=/path/to/DIET")
  message("	- You can use the ccmake GUI")
  set(DIET_DIR "" cache path "DIET installation path")
endif(DIET_FOUND)

find_package(OmniORB)
if (OMNIORB4_FOUND)
else (OMNIORB4_FOUND)
  message("OMNIORB4 installation was not found. Define the OMNIORB4_DIR variable to continue.")
  message(" - You can define a OMNIORB4_DIR environment variable")
  message(" - You can pass it as argument to cmake:")
  message("   $ cmake <source root directory> -DOMNIORB4_DIR:PATH=/path/to/OMNIORB4")
  message(" - You can use the ccmake GUI")
  set(OMNIORB4_DIR "" cache path "OMNIORB4 installation path")
endif(OMNIORB4_FOUND)

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

find_package(POSTGRESQL)
if (POSTGRESQL_FOUND)
  add_definitions(-DUSE_POSTGRES)
else (POSTGRESQL_FOUND)
  message("POSTGRESQL installation was not found. Define the POSTGRESQL_DIR variable to continue.")
  message("     - You can define a POSTGRESQL_DIR environment variable")
  message("     - You can pass it as argument to cmake:")
  message("       $ cmake <source root directory> -DPOSTGRESQL_DIR:PATH=/path/to/POSTGRESQL")
  message("     - You can use the ccmake GUI")
  set(POSTGRESQL_DIR "" cache path "POSTGRESQL installation path")
endif(POSTGRESQL_FOUND)

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
  else (MYSQL_FOUND)
    message("MYSQL installation was not found. Define the MYSQL_DIR variable to continue.")
    message("     - You can define a MYSQL_DIR environment variable")
    message("     - You can pass it as argument to cmake:")
    message("       $ cmake <source root directory> -DMYSQL_DIR:PATH=/path/to/MYSQL")
    message("     - You can use the ccmake GUI")
    set(MYSQL_DIR "" cache path "MYSQL installation path")
  endif(MYSQL_FOUND)
endif (ENABLE_MYSQL)


if (COMPILE_TMS AND NOT CLIENT_ONLY AND NOT VISHNU_USE_LOADLEVELER AND NOT VISHNU_USE_TORQUE)
   message(FATAL_ERROR "You MUST set either VISHNU_USE_TORQUE or VISHNU_USE_LOADLEVELER when compiling TMS server")
endif()

if (VISHNU_USE_TORQUE)
  add_definitions(-DHAVE_TORQUE)

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
endif (VISHNU_USE_TORQUE)

if (VISHNU_USE_LOADLEVELER)
  add_definitions(-DHAVE_LOADLEVELER)

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
endif (VISHNU_USE_LOADLEVELER)

find_package(
        Boost
	1.45
        REQUIRED program_options date_time thread filesystem system unit_test_framework serialization random
)

find_package( Threads )
if( THREADS_FOUND )
  set( THREADS_LIB ${CMAKE_THREAD_LIBS_INIT} )
endif()

set( Boost_LIBRARIES ${Boost_LIBRARIES} ${THREADS_LIB} )

add_definitions( "-DBOOST_FILESYSTEM_VERSION=3" )

#Core
set(EMF4CPP_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/deps/emf4cpp)
set(VISHNU_EXCEPTION_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/exception)
set(DATA_BASE_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/database)
set(REGISTRY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/registry)
set(UTILVISHNU_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/utils)
set(CONFIG_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/config)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/vishnu_version.hpp.in
    ${UTILVISHNU_SOURCE_DIR}/vishnu_version.hpp )
#UMS
set(EMF_DATA_DIR ${VISHNU_SOURCE_DIR}/UMS/src/utils/emfdata)
set(UMS_DATA_DIR ${VISHNU_SOURCE_DIR}/UMS/src/utils/emfdata/UMS_Data)
set(CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/client)
set(SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/server)
set(API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/api)
set(SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/sed)

#TMS
set(TMS_EMF_DATA_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils/emfdata)
set(TMS_DATA_DIR ${VISHNU_SOURCE_DIR}/UMS/src/utils/emfdata/TMS_Data)
set(TMS_CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/client)
set(TMS_SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/server)
set(TMS_API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/api)
set(TMS_CLI_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/cli)
set(TMS_SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/sed)
set(TMS_UTILS_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils/)
set(UTILS_TORQUE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils_torque/)

#IMS
set(IMS_EMF_DATA_DIR ${VISHNU_SOURCE_DIR}/IMS/src/utils/emfdata)
set(IMS_DATA_DIR ${VISHNU_SOURCE_DIR}/IMS/src/utils/emfdata/IMS_Data)
set(IMS_CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/client)
set(IMS_SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/server)
set(IMS_API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/api)
set(IMS_SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/IMS/src/sed)
set(IMS_UTIL_DIR ${VISHNU_SOURCE_DIR}/IMS/src/utils)


# gather executables and libraries under one roof
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set( BIN_DIR ${EXECUTABLE_OUTPUT_PATH} )

# ---------------- SET THE LINKER SEARCH PATH (RPATH) ----------------------
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON
    CACHE BOOL "Whether to set an rpath for dynamic libraries.")
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
    CACHE STRING "Rpath set at install stage." FORCE)

# for testing
set(TESTVISHNU_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/test/src)

add_subdirectory(core/src)
add_subdirectory(core/deps/emf4cpp)

if (DEBUG_TESTS)
  add_definitions( -DDEBUG_TESTS )
endif (DEBUG_TESTS)

if(COMPILE_UMS)
  add_subdirectory(UMS/src)
  add_subdirectory(UMS/doc/man)
  if (BUILD_TESTING)
    add_subdirectory(UMS/test/src)
    set(UMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_UMS)

if(COMPILE_TMS)
  add_subdirectory(TMS/src)
  add_subdirectory(TMS/doc/man)
  if (BUILD_TESTING)
    add_subdirectory(TMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_TMS)

if(COMPILE_IMS)
#  ADD_DEFINITIONS( -DHAVE_BACKGROUND )
  find_package(LOGSERVICE)
  if( LOGSERVICE_FOUND )
    add_definitions(-DUSE_LOG_SERVICE)
    ADD_DEFINITIONS( -DHAVE_ALT_BATCH )
    ADD_DEFINITIONS( -DHAVE_CORI )
    add_subdirectory(IMS/src)
    #if the tests is activated
    if (BUILD_TESTING)
      add_subdirectory(IMS/test/src)
    endif (BUILD_TESTING)
  endif()
endif (COMPILE_IMS)

if (BUILD_TESTING)
  set( TEST_DB_TYPE "postgresql" CACHE STRING "Type of the test database")
  set( TEST_DB_HOST "localhost" CACHE STRING "Hostname of the test database server")
  set( TEST_DB_NAME "vishnu" CACHE STRING "Database name of the test database")
  set( TEST_DB_LOGIN "vishnu" CACHE STRING "Login for the test database")
  set( TEST_DB_PASSWORD "vishnu" CACHE STRING "Password for the test database")
  set( TEST_USER_LOGIN "vishnu" CACHE STRING "Login for local account used for tests")
  set( TEST_ROOT_LOGIN "vishnu" CACHE STRING "Vishnu root login for local account used for tests")
  set( TEST_ADMIN_LOGIN "vishnu" CACHE STRING "Vishnu admin login for local account used for tests")
  set( TEST_USER_SSHKEY "$HOME/.ssh/id_rsa" CACHE STRING "Private key path for local account used for tests")
  set( TEST_ROOT_SSHKEY "$HOME/.ssh/id_rsa" CACHE STRING "Private key path for local account used for tests")
  set( TEST_ADMIN_SSHKEY "$HOME/.ssh/id_rsa" CACHE STRING "Private key path for local account used for tests")
  set( TEST_BATCH_SERVER_HOSTNAME "localhost" CACHE STRING " local hostname used for tests")
  set( TEST_WORKING_DIR ${PROJECT_BINARY_DIR}/test_files CACHE PATH "Tests working directory")
  set( TEST_LOCAL_HOSTNAME "localhost" CACHE STRING " local hostname used for tests")

  mark_as_advanced(TEST_DB_TYPE)
  mark_as_advanced(TEST_DB_HOST)
  mark_as_advanced(TEST_DB_NAME)
  mark_as_advanced(TEST_DB_LOGIN)
  mark_as_advanced(TEST_DB_PASSWORD)
  mark_as_advanced(TEST_WORKING_DIR)
  mark_as_advanced(TEST_ROOT_LOGIN)
  mark_as_advanced(TEST_ROOT_SSHKEY)
  mark_as_advanced(TEST_ADMIN_LOGIN)
  mark_as_advanced(TEST_ADMIN_LOGIN)
  mark_as_advanced(TEST_USER_SSHKEY)
  mark_as_advanced(TEST_USER_SSHKEY)
  mark_as_advanced(TEST_BATCH_SERVER_HOSTNAME)
  mark_as_advanced(TEST_LOCAL_HOSTNAME)


  add_subdirectory(core/test)
endif (BUILD_TESTING)
