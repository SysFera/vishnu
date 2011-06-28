
LINK_DIRECTORIES(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
  .
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${IMS_EMF_DATA_DIR}
  ${IMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${IMS_CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${API_SOURCE_DIR}
  ${IMS_UTIL_DIR}
)

ADD_LIBRARY(
vishnu-ims-api api_ims.cpp
)
SET_TARGET_PROPERTIES(vishnu-ims-api PROPERTIES COMPILE_FLAGS "-fPIC")

TARGET_LINK_LIBRARIES(vishnu-ims-api
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-UMS_Data
  emf4cpp-IMS_Data
  exception-vishnu
  ums-client-proxy
  ims-client-proxy
  utils-vishnu
)

if (COMPILE_TMS)
  TARGET_LINK_LIBRARIES(vishnu-ims-api
    tms-client-proxy
    )
endif(COMPILE_TMS)

INSTALL( TARGETS vishnu-ims-api DESTINATION lib )
INSTALL( FILES api_ims.hpp DESTINATION include)

