
link_directories(
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
)

add_library(ums-client-proxy
   SessionProxy.cpp
   UserProxy.cpp
   MachineProxy.cpp
   LocalAccountProxy.cpp
   ConfigurationProxy.cpp
   OptionValueProxy.cpp
   utilsClient.cpp
   UtilsProxy.cpp
)

target_link_libraries(ums-client-proxy
  ${DIET_CLIENT_LIBRARIES}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  emf4cpp-UMS_Data
  exception-vishnu
  crypt
)

install( TARGETS ums-client-proxy DESTINATION lib )
