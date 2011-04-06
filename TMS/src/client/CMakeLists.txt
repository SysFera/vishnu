
link_directories(
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${TMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${TMS_CLIENT_PROXY_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${TMS_UTILS_DIR}
  )

add_library(tms-client-proxy STATIC
JobProxy.cpp
JobOutPutProxy.cpp
)

SET_TARGET_PROPERTIES(tms-client-proxy PROPERTIES COMPILE_FLAGS "-fPIC")

target_link_libraries(tms-client-proxy
  ${DIET_CLIENT_LIBRARIES}
  ums-client-proxy
  emf4cpp-UMS_Data
  emf4cpp-TMS_Data
  exception-vishnu
  crypt
)
