
link_directories( 
                  ${EMF4CPP_LIB}
                  ${EMF4CPP_CPP_LIB}
                  ${EMF4CPP_LIB} 
                  ${EMF4CPP_CPP_LIB}
                  ${VISHNU_EXCEPTION_DIR}
                )

include_directories(
  ${DIET_DIR}/include
  ${DIET_INCLUDE_DIR}
  ${DIET_DIR}/include
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_CPP_DIR}/include/emf4cpp
  ${EMF4CPP_INCLUDE_DIR} 
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

 
add_library(umsclient
   SessionProxy.cpp 
   UserProxy.cpp
)

target_link_libraries(umsclient
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  ${EMF4CPP_LIB} 
  ${EMF4CPP_CPP_LIB}
  ${VISHNU_EXCEPTION_LIB}
 #exception-vishnu
)

