
link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
)


include_directories(
  .
  ${DIET_INCLUDE_DIR}
  ${IMS_EMF_DATA_DIR}
  ${IMS_DATA_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${IMS_UTIL_DIR}
)

add_library(vishnu-ims-api
  api_ims.cpp
)

SET_TARGET_PROPERTIES(vishnu-ims-api PROPERTIES COMPILE_FLAGS "-fPIC")

target_link_libraries(vishnu-ims-api
  ${DIET_CLIENT_LIB}
  ${Boost_LIBRARIES}
  emf4cpp-IMS_Data
  exception-vishnu
  utils-vishnu
  ims-client-proxy
  ums-client-proxy
)


# Java and/or python are options
if (ENABLE_PYTHON OR ENABLE_JAVA)

# Requirements for PYTHON
FIND_PACKAGE(SWIG 1.3.40 REQUIRED)
INCLUDE(${SWIG_USE_FILE})
MARK_AS_ADVANCED(SWIG_DIR)
MARK_AS_ADVANCED(SWIG_EXECUTABLE)
MARK_AS_ADVANCED(SWIG_VERSION)

if(ENABLE_PYTHON)
FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
endif(ENABLE_PYTHON)

# Requirements for JAVA
if(ENABLE_JAVA)
FIND_PACKAGE(Java REQUIRED)
FIND_PATH(JAVA_INCLUDE_DIR jni.h
  $ENV{JAVA_HOME}/include
  /usr/lib/jvm/java/include
  /usr/lib/jvm/java-6*/include
  /usr/lib/jvm/java-6-sun-1.6.0.22/include
)
INCLUDE_DIRECTORIES(
  ${JAVA_INCLUDE_DIR}
  ${JAVA_INCLUDE_DIR}/linux
)
endif(ENABLE_JAVA)

# General SWIG configuration
SET_SOURCE_FILES_PROPERTIES(api_ims.i PROPERTIES CPLUSPLUS ON)
#Install swig language-specific generated files in a dedicated directory
SET (CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/swig_output)
file(MAKE_DIRECTORY ${CMAKE_SWIG_OUTDIR})
#Suppress warnings about ignored operators (out_ptr::operator= and EList::operator[])
SET(CMAKE_SWIG_FLAGS "-w362,389")

## PYTHON MODULE
#The module name must match the %module declaration in the interface file
if(ENABLE_PYTHON)
SWIG_ADD_MODULE(VISHNU_IMS python api_ims.i api_ims.cpp)
SET_TARGET_PROPERTIES(${SWIG_MODULE_VISHNU_IMS_REAL_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
SWIG_LINK_LIBRARIES(VISHNU_IMS ${PYTHON_LIBRARIES} ${DIET_CLIENT_LIB} vishnu-ims-api exception-vishnu)
INSTALL( TARGETS ${SWIG_MODULE_VISHNU_IMS_REAL_NAME} DESTINATION lib )
endif(ENABLE_PYTHON)

## JAVA MODULE
if(ENABLE_JAVA)
SET(CMAKE_SWIG_FLAGS "-w503" "-package" "com.sysfera.vishnu.api.ims.internal")
SWIG_ADD_MODULE(VISHNU_IMS java api_ims.i api_ims.cpp)
SET_TARGET_PROPERTIES(${SWIG_MODULE_VISHNU_IMS_REAL_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")

SWIG_LINK_LIBRARIES(VISHNU_IMS ${JAVA_LIBRARIES} ${DIET_CLIENT_LIB} vishnu-ims-api exception-vishnu)
INSTALL( TARGETS ${SWIG_MODULE_VISHNU_IMS_REAL_NAME} DESTINATION lib )
endif(ENABLE_JAVA)


INSTALL( TARGETS vishnu-ims-api DESTINATION lib )
INSTALL( DIRECTORY ${CMAKE_SWIG_OUTDIR} DESTINATION lib)
endif(ENABLE_PYTHON OR ENABLE_JAVA)
INSTALL( FILES api_ims.hpp DESTINATION include)

