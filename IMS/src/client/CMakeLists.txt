
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
  ${FMS_API_SOURCE_DIR}
)

add_library(ims-client-proxy STATIC
  MetricProxy.cpp
  ObjectIdProxy.cpp
  SysInfoProxy.cpp
  ThresholdProxy.cpp
  ProcessCtlProxy.cpp
  ExporterProxy.cpp
)

SET_TARGET_PROPERTIES(ims-client-proxy PROPERTIES COMPILE_FLAGS "-fPIC")
target_link_libraries(ims-client-proxy
  ${DIET_CLIENT_LIB}
  ${Boost_LIBRARIES}
  emf4cpp-IMS_Data
  emf4cpp-TMS_Data
  emf4cpp-FMS_Data
  exception-vishnu
  ums-client-proxy
)

if (COMPILE_FMS)
  target_link_libraries(ims-client-proxy
    vishnu-fms-api
    )
endif (COMPILE_FMS)