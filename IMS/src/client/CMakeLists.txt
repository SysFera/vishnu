 
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
  ${CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
)

add_library(ims-client-proxy STATIC
  MetricProxy.cpp
)

target_link_libraries(ims-client-proxy
  ${DIET_CLIENT_LIB}
  ${Boost_LIBRARIES}
  emf4cpp-IMS_Data
  exception-vishnu
  utils-vishnu
#  ims-client-proxy
  ums-client-proxy
)

