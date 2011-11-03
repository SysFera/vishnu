
link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

include_directories(
  .
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${FMS_EMF_DATA_DIR}
  ${FMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
  ${FMS_CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${API_SOURCE_DIR}
)

add_library(
vishnu-fms-api api_fms.cpp
)
set_target_properties(vishnu-fms-api PROPERTIES COMPILE_FLAGS "-fPIC")

target_link_libraries(vishnu-fms-api
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-FMS_Data
  exception-vishnu
  ums-client-proxy
  fms-client-proxy
  utils-vishnu
)


install(TARGETS vishnu-fms-api DESTINATION ${LIB_INSTALl_DIR})
install(FILES api_fms.hpp DESTINATION ${INCLUDE_INSTALL_DIR})

