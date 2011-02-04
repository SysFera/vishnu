
link_directories( 
 ${VISHNU_EXCEPTION_INCLUDE_DIR}                  
)

include_directories(
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

