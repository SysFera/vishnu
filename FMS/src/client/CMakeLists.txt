
link_directories(
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${OMNIORB4_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${FMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${FMS_CLIENT_PROXY_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${FMS_UTILS_DIR}
  )

add_library(fms-client-proxy STATIC
  FileProxy
  LocalFileProxy
  RemoteFileProxy
  FileProxyFactory
)

SET_TARGET_PROPERTIES(fms-client-proxy PROPERTIES COMPILE_FLAGS "-fPIC")

target_link_libraries(fms-client-proxy
  ${DIET_CLIENT_LIBRARIES}
  ums-client-proxy
  emf4cpp-UMS_Data
  emf4cpp-FMS_Data
  exception-vishnu
  crypt
)
