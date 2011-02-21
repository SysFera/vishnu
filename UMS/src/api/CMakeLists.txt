
link_directories( 
 ${VISHNU_EXCEPTION_INCLUDE_DIR}                  
)

include_directories(
  .
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR} 
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
)

add_library(ums-api-client
   api_ums.cpp 
)

target_link_libraries(ums-api-client
  ${DIET_CLIENT_LIBRARIES}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  emf4cpp-UMS_Data
  exception-vishnu
  ums-client-proxy
)

FIND_PACKAGE(SWIG 1.3.40 REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

SET_SOURCE_FILES_PROPERTIES(api_ums.i PROPERTIES CPLUSPLUS ON)

#Suppress warnings about ignored operators (out_ptr::operator= and EList::operator[])
SET(CMAKE_SWIG_FLAGS "-w362,389")
#SET(CMAKE_SWIG_FLAGS "")

#The module name must match the %module declaration in the interface file
#This is why the initial name 'APIPython' was removed and replaced by APITest
SWIG_ADD_MODULE(VISHNU_UMS python api_ums.i api_ums.cpp)
SWIG_LINK_LIBRARIES(VISHNU_UMS ${PYTHON_LIBRARIES} ${DIET_CLIENT_LIB} ums-api-client)
