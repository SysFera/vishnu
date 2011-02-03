
find_path(EMF4CPP_INCLUDE_DIR ecorecpp.hpp
  paths
  ${EMF4CPP_CPP_DIR}/include/emf4cpp
  $ENV{EMF4CPP_CPP_DIR}/include/emf4cpp
  /opt/emf4cpp-0.0.2/include/emf4cpp
  /usr/include/emf4cpp
  /usr/local/include/emf4cpp
  /opt/local/include/emf4cpp
)

find_library(EMF4CPP_LIB emf4cpp-ecore
  ${EMF4CPP_CPP_DIR}/lib
  $ENV{EMF4CPP_CPP_DIR}/lib
  /opt/emf4cpp-0.0.2/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(EMF4CPP_CPP_LIB emf4cpp-ecorecpp
  ${EMF4CPP_CPP_DIR}/lib
  $ENV{EMF4CPP_CPP_DIR}/lib
  /opt/emf4cpp-0.0.2/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(EMF4CPP_UMS_DATA_LIB emf4cpp-UMS_Data
  ${EMF4CPP_UMS_DATA_DIR}/lib
  $ENV{EMF4CPP_UMS_DATA_DIR}/lib
  /opt/emf4cpp-0.0.2/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  ${UserManagementSystem}/src/utils/build
)

if (EMF4CPP_INCLUDE_DIR AND  EMF4CPP_LIB AND EMF4CPP_CPP_LIB AND EMF4CPP_UMS_DATA_LIB)
  set(EMF_FOUND "Yes")
  mark_as_advanced(EMF4CPP_INCLUDE_DIR)
  mark_as_advanced(EMF4CPP_LIB)
  mark_as_advanced(EMF4CPP_CPP_LIB)
  mark_as_advanced(EMF4CPP_UMS_DATA_LIB)
endif(EMF4CPP_INCLUDE_DIR AND  EMF4CPP_LIB AND EMF4CPP_CPP_LIB AND EMF4CPP_UMS_DATA_LIB)

if (EMF_FOUND)
  else (EMF_FOUND)
    message("EMF installation was not found. Define the EMF_DIR variable to continue.")
    message("     - You can define a EMF_DIR environment variable")
    message("     - You can pass it as argument to cmake:")
    message("       $ cmake <source root directory> -DEMF_DIR:PATH=/path/to/EMF")
    message("     - You can use the ccmake GUI")
    set(EMF_DIR "" cache path "EMF installation path")
endif(EMF_FOUND)

