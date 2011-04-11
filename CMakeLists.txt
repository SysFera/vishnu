# Root CMakeLists.txt file
#
# Project: VISHNU
# Author: E. PAMBA CAPO-CHICHI
# Date: 4th February 2011

cmake_minimum_required(VERSION 2.6)
project(VISHNU)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH ${VISHNU_SOURCE_DIR}/Cmake)
set (MANDIR /usr/share/man CACHE string "the man path")

option(BUILD_SHARED_LIBS ON "disable shared libraries usage")
option(CLIENT_ONLY OFF "only compile the client side")
option(ENABLE_PYTHON OFF "compile the python api")
option(ENABLE_JAVA OFF "compile the java api")
option(ENABLE_ORACLE OFF "compile with the oracle support. Deprecated")
option(COMPILE_UMS OFF "compile the module UMS")
option(COMPILE_TMS OFF "compile the module TMS")
option(VISHNU_USE_TORQUE   "Build VISHNU with Torque support..." OFF)
option(VISHNU_USE_LOADLEVELER   "Build VISHNU with LoadLeveler support..." OFF)

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


find_package(
        Boost
	1.45
        REQUIRED program_options date_time thread filesystem system unit_test_framework serialization random
)
add_definitions( "-DBOOST_FILESYSTEM_VERSION=3" )

set(EMF4CPP_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/deps/emf4cpp)
set(EMF_DATA_DIR ${VISHNU_SOURCE_DIR}/UMS/src/utils/emfdata)
set(UMS_DATA_DIR ${VISHNU_SOURCE_DIR}/UMS/src/utils/emfdata/UMS_Data)
set(VISHNU_EXCEPTION_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/exception)
set(DATA_BASE_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/database)
set(CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/client)
set(SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/server)
set(API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/api)
set(CLI_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/cli)
set(SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/src/sed)
set(REGISTRY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/registry)
set(UTILVISHNU_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/utils)

set(TMS_EMF_DATA_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils/emfdata)
set(TMS_DATA_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils/emfdata/TMS_Data)
set(TMS_CLIENT_PROXY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/client)
set(TMS_SERVER_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/server)
set(TMS_API_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/api)
set(TMS_CLI_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/cli)
set(TMS_SED_SOURCE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/sed)
set(TMS_UTILS_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils/)
set(UTILS_TORQUE_DIR ${VISHNU_SOURCE_DIR}/TMS/src/utils_torque/)

# gather executables and libraries under one roof
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set( BIN_DIR ${EXECUTABLE_OUTPUT_PATH} )
set( UMS_TEST_DIR ${VISHNU_SOURCE_DIR}/UMS/test/src/sql )

set (TEST_CONFIG_UMS "${PROJECT_BINARY_DIR}/include")
set (TEST_SQL "${UMS_TEST_DIR}")

add_subdirectory(core/src)
add_subdirectory(core/deps/emf4cpp)

if(COMPILE_UMS)
  add_subdirectory(UMS/src)
  add_subdirectory(UMS/doc/man)
endif (COMPILE_UMS)
# macros used to setup test environment
#include(UMSTestingMacros)
#ums_test_setup()
#add_subdirectory(UMS/test/src)

if(COMPILE_TMS)
  add_subdirectory(TMS/src)
endif (COMPILE_TMS)

if (BUILD_TEST)
  add_subdirectory(core/test)
endif (BUILD_TEST)
