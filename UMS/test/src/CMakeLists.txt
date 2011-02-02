project(tests)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH cmake)

set (Boost_LIBRARYDIR /opt/boost_1_45_0/libs)

find_package( Boost 1.45.0 COMPONENTS
  unit_test_framework
  filesystem
  system
  thread REQUIRED )
set( Boost_MULTITHREADED ON )

#set( EMF_INC_DIR "")
find_path( UMS_GEN_DIR "UMS_Data.hpp")
find_path( EMF_INC_DIR "ecore_forward.hpp"
  /opt/emf4cpp-0.0.2/include/emf4cpp)

find_library(EMF4CPP_LIB emf4cpp-ecore
  /opt/emf4cpp-0.0.2/lib
)

find_library(EMF4CPP_CPP_LIB emf4cpp-ecorecpp
  /opt/emf4cpp-0.0.2/lib
)

set (UMSPATH "")
find_library( UMS_LIB emf4cpp-UMS_Data
  ${UMSPATH})

#/home/keo/Bureau/depot_git_edf/vishnu/UMS/src/utils/emfdata/build/libemf4cpp-UMS_Data.so

include_directories( ${Boost_INCLUDE_DIRS} 
  ${EMF_INC_DIR}
  ${UMS_GEN_DIR}
  )

link_directories ( ${Boost_LIBRARY_DIRS})


add_executable( automTest automTest.cpp TestRunner.cpp)

target_link_libraries( automTest
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  ${UMS_LIB}
)

add_test( MyTest automTest )
