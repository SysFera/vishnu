
include_directories(
  ${DIET_DIR}/include
  ${DIET_INCLUDE_DIR}
  ${DIET_DIR}/include
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_CPP_DIR}/include/emf4cpp
  ${EMF4CPP_INCLUDE_DIR} 
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
)

 
add_library(umsserver
   SessionServer.cc
   UserServer.cc
   MachineClientServer.cc
)

target_link_libraries(umsserver
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  ${EMF4CPP_LIB} 
  ${EMF4CPP_CPP_LIB}
  ${LIBPQ_LIB}
  ${VISHNU_EXCEPTION_LIB}
  ${DATA_BASE_LIB}
  #database-vishnu
  #exception-vishnu
)

