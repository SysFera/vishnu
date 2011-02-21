
link_directories( 
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${Boost_LIBRARY_DIRS}		    
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR} 
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

 
add_library(ums-server
   utilServer.cpp
   SessionServer.cpp
   UserServer.cpp
   MachineClientServer.cpp
   OptionValueServer.cpp
   MachineServer.cpp 
   LocalAccountServer.cpp
   ConfigurationServer.cpp 
)

target_link_libraries(ums-server
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  ${EMF4CPP_LIB} 
  ${EMF4CPP_CPP_LIB}
  ${LIBPQ_LIB}
  ${Boost_LIBRARIES}
  ${LIBCRYPT_LIB}
  emf4cpp-UMS_Data
  database-vishnu
  exception-vishnu
)

