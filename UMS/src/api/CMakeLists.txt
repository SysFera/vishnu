
LINK_DIRECTORIES(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
  .
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
)

ADD_LIBRARY(ums-api-client
   api_ums.cpp
)

TARGET_LINK_LIBRARIES(ums-api-client
  ${DIET_CLIENT_LIBRARIES}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  emf4cpp-UMS_Data
  exception-vishnu
  ums-client-proxy
)

# Requirements for PYTHON
FIND_PACKAGE(SWIG 1.3.40 REQUIRED)
INCLUDE(${SWIG_USE_FILE})
MARK_AS_ADVANCED(SWIG_DIR)
MARK_AS_ADVANCED(SWIG_EXECUTABLE)
MARK_AS_ADVANCED(SWIG_VERSION)

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

# Requirements for JAVA
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

# General SWIG configuration
SET_SOURCE_FILES_PROPERTIES(api_ums.i PROPERTIES CPLUSPLUS ON)
#Install swig language-specific generated files in a dedicated directory
SET (CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/swig_output)
#Suppress warnings about ignored operators (out_ptr::operator= and EList::operator[])
SET(CMAKE_SWIG_FLAGS "-w362,389")

## PYTHON MODULE
#The module name must match the %module declaration in the interface file
SWIG_ADD_MODULE(VISHNU_UMS python api_ums.i api_ums.cpp)
SWIG_LINK_LIBRARIES(VISHNU_UMS ${PYTHON_LIBRARIES} ${DIET_CLIENT_LIB} ums-api-client exception-vishnu)
INSTALL( TARGETS ${SWIG_MODULE_VISHNU_UMS_REAL_NAME} DESTINATION lib )

## JAVA MODULE
SET(CMAKE_SWIG_FLAGS "-w503" "-package" "com.sysfera.vishnu.ws")
SWIG_ADD_MODULE(VISHNU_UMS java api_ums.i api_ums.cpp)
SWIG_LINK_LIBRARIES(VISHNU_UMS ${JAVA_LIBRARIES} ${DIET_CLIENT_LIB} ums-api-client exception-vishnu)
INSTALL( TARGETS ${SWIG_MODULE_VISHNU_UMS_REAL_NAME} DESTINATION lib )

INSTALL( TARGETS ums-api-client DESTINATION lib )
INSTALL( DIRECTORY ${CMAKE_SWIG_OUTDIR} DESTINATION lib)
INSTALL( FILES api_ums.hpp DESTINATION include)
