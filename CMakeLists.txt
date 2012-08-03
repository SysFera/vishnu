# Root CMakeLists.txt file
#
# Project: VISHNU
# Author: E. PAMBA CAPO-CHICHI
# Date: 4th February 2011

cmake_minimum_required(VERSION 2.6)
project(VISHNU)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Cmake)

set( VISHNU_VERSION "2.0.0" CACHE STRING " the version of the source ")

# Options set to ON by default
option(BUILD_SHARED_LIBS "disable shared libraries usage" ON)


# COMPILE_UMS option set to ON by default
option(COMPILE_UMS "compile the module UMS" ON)

# Options set to OFF by default
option(CLIENT_ONLY "only compile the client side")
option(ENABLE_PYTHON "compile the python api")
option(ENABLE_JAVA "compile the java api")
option(ENABLE_SWIG "use swig to re-generate code for the wrappers")
option(COMPILE_TMS "compile the module TMS")
option(COMPILE_FMS "compile the module FMS")
option(COMPILE_IMS "compile the module IMS")
option(VISHNU_USE_LOADLEVELER   "Build VISHNU with LoadLeveler support...")
option(VISHNU_USE_TORQUE   "Build VISHNU with Torque support...")
option(VISHNU_USE_SLURM   "Build VISHNU with SLURM support...")
option(VISHNU_USE_LSF   "Build VISHNU with LSF support...")
option(VISHNU_USE_SGE     "Build VISHNU with SGE support...")
option(VISHNU_USE_LDAP   "Build VISHNU with LDAP support...")


if (COMPILE_UMS AND COMPILE_TMS AND COMPILE_FMS AND COMPILE_IMS)
  set(COMPILE_ALL_MODULES 1)
endif (COMPILE_UMS AND COMPILE_TMS AND COMPILE_FMS AND COMPILE_IMS)

# FIXME: cache is not updated when CLIENT_ONLY is enabled and BUILD_TESTING previously set to ON
include(CMakeDependentOption)
cmake_dependent_option(BUILD_TESTING "Enable unit testing" OFF "NOT CLIENT_ONLY" OFF)
option(DEBUG_TESTS "Provide debug info during tests execution" OFF)

find_package(DIET)
if (DIET_FOUND)
else (DIET_FOUND)
  message("DIET installation was not found. Define the DIET_DIR variable to continue.")
  message(" - You can define a DIET_DIR environment variable")
  message(" - You can pass it as argument to cmake:")
  message("   $ cmake <source root directory> -DDIET_DIR:PATH=/path/to/DIET")
  message(" - You can use the ccmake GUI")
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

set(DIET_CLIENT_LIBRARIES ${DIET_CLIENT_LIBRARIES} ${OMNIORB4_LIBRARIES})
set(DIET_SERVER_LIBRARIES ${DIET_SERVER_LIBRARIES}
  ${DIET_ADMIN_LIBRARIES}
  ${OMNIORB4_LIBRARIES})

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


find_package(SOCI)
if (SOCI_FOUND)
  set(DATABASE_LIBS ${DATABASE_LIBS} ${SOCI_LIBS})
  set(DATABASE_INCLUDE_DIR ${DATABASE_INCLUDE_DIR} ${SOCI_INCLUDE_DIR})
else (SOCI_FOUND)
  message("SOCI installation was not found. Define the SOCI_DIR variable to continue.")
  message("     - You can define a SOCI_DIR environment variable")
  message("     - You can pass it as argument to cmake:")
  message("       $ cmake <source root directory> -DSOCI_DIR:PATH=/path/to/SOCI")
  message("     - You can use the ccmake GUI")
  set(SOCI_DIR "" cache path "SOCI installation path")
endif(SOCI_FOUND)


if (COMPILE_TMS AND NOT CLIENT_ONLY AND NOT VISHNU_USE_LOADLEVELER AND NOT VISHNU_USE_TORQUE AND NOT VISHNU_USE_SLURM AND NOT VISHNU_USE_LSF AND NOT VISHNU_USE_SGE)
   message(FATAL_ERROR "You MUST set either VISHNU_USE_TORQUE or VISHNU_USE_LOADLEVELER or VISHNU_USE_SLURM or VISHNU_USE_LSF or VISHNU_USE_SGE when compiling TMS server")
endif()

if (COMPILE_TMS)
  add_definitions(-DCOMPILE_TMS)
endif(COMPILE_TMS)

if (COMPILE_FMS)
  add_definitions(-DCOMPILE_FMS)
endif(COMPILE_FMS)


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

if (VISHNU_USE_SLURM)

  add_definitions(-DHAVE_SLURM)

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
endif (VISHNU_USE_SLURM)

if (VISHNU_USE_LSF)

  add_definitions(-DHAVE_LSF)

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
endif (VISHNU_USE_LSF)

if (VISHNU_USE_SGE)

  add_definitions(-DHAVE_SGE)

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
endif (VISHNU_USE_SGE)

if (VISHNU_USE_LDAP AND NOT CLIENT_ONLY)
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
endif(VISHNU_USE_LDAP AND NOT CLIENT_ONLY)


find_package(LOGSERVICE REQUIRED)

find_package(Boost 1.45
  REQUIRED program_options date_time thread filesystem system unit_test_framework serialization random)

find_package(Threads)
if(THREADS_FOUND)
  set(THREADS_LIB ${CMAKE_THREAD_LIBS_INIT})
endif()

set(Boost_LIBRARIES ${Boost_LIBRARIES} ${THREADS_LIB})

add_definitions("-DBOOST_FILESYSTEM_VERSION=3")

#Core
set(EMF4CPP_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/deps/emf4cpp)
set(VISHNU_EXCEPTION_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/exception)
set(DATA_BASE_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/database)
set(REGISTRY_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/registry)
set(UTILVISHNU_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/utils)
set(CONFIG_SOURCE_DIR ${VISHNU_SOURCE_DIR}/core/src/config)
set(EMF_DATA_DIR ${VISHNU_SOURCE_DIR}/core/src/emfdata)
set(AUTHENTICATOR_INCLUDE_DIR ${VISHNU_SOURCE_DIR}/core/src/authenticator)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vishnu_version.hpp.in
    ${PROJECT_BINARY_DIR}/vishnu_version.hpp)
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
set(MAN_INSTALL_DIR "share/man"
  CACHE INTERNAL "Directory where man pages will be installed")
set(CMAKE_MOD_INSTALL_DIR "share/cmake/Modules"
  CACHE INTERNAL "Directory where cmake modules will be installed")

add_subdirectory(core/src)
add_subdirectory(core/deps)

if (DEBUG_TESTS)
  add_definitions(-DDEBUG_TESTS)
endif (DEBUG_TESTS)

if (BUILD_TESTING)
  add_definitions(-DBUILD_TESTING)

  set(TEST_DB_TYPE "postgresql" CACHE STRING "Type of the test database")
  set(TEST_DB_HOST "localhost" CACHE STRING "Hostname of the test database server")
  set(TEST_DB_NAME "vishnu" CACHE STRING "Database name of the test database")
  set(TEST_DB_LOGIN "vishnu" CACHE STRING "Login for the test database")
  set(TEST_DB_PASSWORD "vishnu" CACHE STRING "Password for the test database")
  set(TEST_USER_LOGIN "vishnu" CACHE STRING "Login for local account used for tests")
  set(TEST_ROOT_LOGIN "vishnu" CACHE STRING "Vishnu root login for local account used for tests")
  set(TEST_ADMIN_LOGIN "vishnu" CACHE STRING "Vishnu admin login for local account used for tests")
  set(TEST_USER_SSHKEY "$HOME/.ssh/id_rsa" CACHE STRING "Private key path for local account used for tests")
  set(TEST_ROOT_SSHKEY "$HOME/.ssh/id_rsa" CACHE STRING "Private key path for local account used for tests")
  set(TEST_ADMIN_SSHKEY "$HOME/.ssh/id_rsa" CACHE STRING "Private key path for local account used for tests")
  set(TEST_USER_HOME "$HOME" CACHE STRING "Vishnu user home directory for local account used for tests")
  set(TEST_ROOT_HOME "$HOME" CACHE STRING "Vishnu root home directory for local account used for tests")
  set(TEST_ADMIN_HOME "$HOME" CACHE STRING "Vishnu admin home directory for local account used for tests")
  set(TEST_BATCH_SERVER_HOSTNAME "localhost" CACHE STRING " local hostname used for tests")
  set(TEST_BATCH_TYPE "TORQUE" CACHE STRING " batch scheduler type")
  set(TEST_BATCH_TYPE2 "SLURM" CACHE STRING " batch scheduler type")
  set(TEST_WORKING_DIR ${PROJECT_BINARY_DIR}/test_files CACHE PATH "Tests working directory")
  set(TEST_LOCAL_HOSTNAME "localhost" CACHE STRING " local hostname used for tests")
  set(TEST_REMOTE_HOSTNAME "remoteHostname" CACHE STRING " remote hostname used for tests")
  set(TEST_OMNINAMES_CFG_IP "127.0.0.1" CACHE STRING " The omniorb cfg ip for tests")
  set(TEST_TMS_FAST_SCRIPT_NAME fast_torque_script CACHE STRING " The name of the fast script to submit")
  set(TEST_TMS_FAST_SCRIPT_OUTPUTDIR_NAME fast_torque_outputdir_script CACHE STRING " The name of the fast script containing the VISHNU_OUTPUTDIR variable to submit")
  set(TMS_FAST_SCRIPT_PATH ${VISHNU_SOURCE_DIR}/TMS/test/src/scripts CACHE STRING " The path to the fast script to submit")
  set(TEST_TMS_SCRIPT_NAME testScriptGen CACHE STRING " The name of the script to submit")
  set(TEST_TMS_SCRIPT_NAME_WAITING testScriptGenWaiting CACHE STRING " The name of the with an option to put job on waiting state")
  set(TMS_SCRIPT_PATH ${VISHNU_SOURCE_DIR}/TMS/test/src/scripts CACHE STRING " The path to the script to submit")
  set(TEST_CONFIG_DIR ${VISHNU_SOURCE_DIR}/TMS/test/src/scripts/ CACHE PATH "Tests configuration directory")
  set(LSF_QUEUES_CONFIG_DIR /opt/lsf7u6/conf/lsbatch/cluster1/configdir/lsb.queues CACHE PATH "LSF queues configuration directory")
  set(TEST_AUTHENTYPE "UMS" CACHE STRING "The type of authentication")
  set(TEST_ROOT_UMS_LOGIN "root" CACHE STRING "Vishnu root login registered on the user-authentication system and used for tests")
  set(TEST_ROOT_UMS_PWD "vishnu_user" CACHE STRING "Vishnu root password registered on the user-authentication system and used for tests")
  set(TEST_AUTHSYTEM_URI "ldap://127.0.0.1:389/" CACHE STRING "The URI of the authentication system")
  set(TEST_AUTHSYTEM_BASE "uid=$USERNAME,ou=users,dc=edf,dc=fr" CACHE STRING "The base of the ldap authentication system")
  set(TEST_ADMIN_UMS_LOGIN "admin_1" CACHE STRING "Vishnu admin login registered on the user-authentication system and used for tests")
  set(TEST_ADMIN_UMS_PWD "admin" CACHE STRING "Vishnu admin password registered on the user-authentication system and used for tests")
  set(TEST_USER_UMS_LOGIN "user_1" CACHE STRING "Vishnu user login registered on the user-authentication system and used for tests")
  set(TEST_USER_UMS_PWD "toto" CACHE STRING "Vishnu user password registered on the user-authentication system and used for tests")

    

  mark_as_advanced(TEST_DB_TYPE)
  mark_as_advanced(TEST_DB_HOST)
  # mark_as_advanced(TEST_DB_NAME)
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
  mark_as_advanced(TEST_REMOTE_HOSTNAME)
  mark_as_advanced(TEST_OMNINAMES_CFG_IP)
  mark_as_advanced(TEST_ROOT_UMS_LOGIN)
  mark_as_advanced(TEST_ROOT_UMS_PWD)
  mark_as_advanced(TEST_AUTHSYTEM_URI)
  mark_as_advanced(TEST_AUTHSYTEM_BASE)
  mark_as_advanced(TEST_ADMIN_UMS_LOGIN)
  mark_as_advanced(TEST_ADMIN_UMS_PWD)
  mark_as_advanced(TEST_USER_UMS_LOGIN)
  mark_as_advanced(TEST_USER_UMS_PWD)


  # Common directories for tests
  set(TEST_SQL_PATH "${PROJECT_BINARY_DIR}/test_files/sql" CACHE STRING "Contains SQL for tests")
  set(TEST_CONFIG_PATH "${PROJECT_BINARY_DIR}/test_files/cfg" CACHE STRING "Contains configurations for tests")
  set(TEST_BIN_PATH "${PROJECT_BINARY_DIR}/bin" CACHE STRING "Contains binaries for tests")
  mark_as_advanced(TEST_SQL_PATH)
  mark_as_advanced(TEST_CONFIG_PATH)
  mark_as_advanced(TEST_BIN_PATH)

  add_subdirectory(core/test)
endif (BUILD_TESTING)

if (ENABLE_PYTHON OR ENABLE_JAVA)
  add_subdirectory(swigAPI)
endif(ENABLE_PYTHON OR ENABLE_JAVA)

if(COMPILE_UMS)
  add_subdirectory(UMS/src)
  add_subdirectory(UMS/doc/man)
  add_definitions(-DCOMPILE_UMS)
  if (BUILD_TESTING)
    add_subdirectory(UMS/test/src)
    set(UMS_TEST_SOURCE_DIR ${VISHNU_SOURCE_DIR}/UMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_UMS)

if(COMPILE_TMS)
  add_subdirectory(TMS/src)
  add_subdirectory(TMS/doc/man)
  add_definitions(-DCOMPILE_TMS)
  if (BUILD_TESTING)
    add_subdirectory(TMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_TMS)

if(COMPILE_IMS)
  add_definitions(-DCOMPILE_IMS)
  add_definitions(-DUSE_LOG_SERVICE)
  add_definitions(-DHAVE_ALT_BATCH)
  add_definitions(-DHAVE_CORI)
  add_subdirectory(IMS/src)
  add_subdirectory(IMS/doc/man)
  if (BUILD_TESTING)
    add_subdirectory(IMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_IMS)

if(COMPILE_FMS)
  add_subdirectory(FMS/src)
  add_subdirectory(FMS/doc/man)
  add_definitions(-DCOMPILE_FMS)
  if (BUILD_TESTING)
      add_subdirectory(FMS/test/src)
  endif (BUILD_TESTING)
endif (COMPILE_FMS)


# module that adds an uninstall target
include(CMakeUninstall)