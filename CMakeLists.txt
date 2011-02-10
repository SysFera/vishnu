# Root CMakeLists.txt file
#
# Project: UserManagementSystem
# Author: E. PAMBA CAPO-CHICHI
# Date: 4th February 2011

cmake_minimum_required(VERSION 2.8)
project(UserManagementSystem)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH ${UserManagementSystem_SOURCE_DIR}/Cmake)

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

find_package(LIBPQ)
if (LIBPQ_FOUND)
else (LIBPQ_FOUND)
  message("LIBPQ installation was not found. Define the LIBPQ_DIR variable to continue.")
  message("     - You can define a LIBPQ_DIR environment variable")
  message("     - You can pass it as argument to cmake:")
  message("       $ cmake <source root directory> -DLIBPQ_DIR:PATH=/path/to/LIBPQ")
  message("     - You can use the ccmake GUI")
  set(LIBPQ_DIR "" cache path "LIBPQ installation path")
endif(LIBPQ_FOUND)

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

find_package(EMF4CPP)
if (EMF4CPP_FOUND)
else (EMF4CPP_FOUND)
  message("EM4CPP installation was not found. Define the EMF4CPP_DIR variable to continue.")
  message("     - You can define a EMF4CPP_DIR environment variable")
  message("     - You can pass it as argument to cmake:")
  message("       $ cmake <source root directory> -DEMF4CPP_DIR:PATH=/path/to/EMF4CPP")
  message("     - You can use the ccmake GUI")
  set(EMF4CPP_DIR "" cache path "EMF4CPP installation path")
endif(EMF4CPP_FOUND)

include(FindBoost)
find_package(
        Boost
        1.40.0
        REQUIRED program_options
)

set(EMF_DATA_DIR ${UserManagementSystem_SOURCE_DIR}/UMS/src/utils/emfdata)
set(UMS_DATA_DIR ${UserManagementSystem_SOURCE_DIR}/UMS/src/utils/emfdata/UMS_Data)
set(VISHNU_EXCEPTION_INCLUDE_DIR ${UserManagementSystem_SOURCE_DIR}/core/exception)
set(DATA_BASE_INCLUDE_DIR ${UserManagementSystem_SOURCE_DIR}/core/database)
set(CLIENT_PROXY_SOURCE_DIR ${UserManagementSystem_SOURCE_DIR}/UMS/src/client)
set(SERVER_SOURCE_DIR ${UserManagementSystem_SOURCE_DIR}/UMS/src/server)
set(API_SOURCE_DIR ${UserManagementSystem_SOURCE_DIR}/UMS/src/api)
set(CLI_SOURCE_DIR ${UserManagementSystem_SOURCE_DIR}/UMS/src/cli)
set(SED_SOURCE_DIR ${UserManagementSystem_SOURCE_DIR}/UMS/src/sed)

add_subdirectory(core/database)
add_subdirectory(UMS/src)
