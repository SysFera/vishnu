
link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${Boost_LIBRARY_DIRS}
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${TMS_EMF_DATA_DIR}
  ${TMS_DATA_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${TMS_CLIENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${TMS_API_SOURCE_DIR}
  ${TMS_CLIENT_PROXY_SOURCE_DIR}
  ${CLIENT_PROXY_SOURCE_DIR}
)


add_executable(test_client
  test_client.cpp
)

target_link_libraries(test_client
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-UMS_Data
  emf4cpp-TMS_Data
  register-vishnu
  exception-vishnu
  database-vishnu
  utils-vishnu
  vishnu-tms-api
  ums-client-proxy
  tms-client-proxy
)

add_executable(test_submit
  test_submit.cpp
)

target_link_libraries(test_submit
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-UMS_Data
  emf4cpp-TMS_Data
  register-vishnu
  exception-vishnu
  database-vishnu
  utils-vishnu
  vishnu-tms-api
  ums-client-proxy
  tms-client-proxy
)

add_executable(test_cancel
  test_cancel.cpp
)

target_link_libraries(test_cancel
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-UMS_Data
  emf4cpp-TMS_Data
  register-vishnu
  exception-vishnu
  database-vishnu
  utils-vishnu
  vishnu-tms-api
  ums-client-proxy
  tms-client-proxy
)
