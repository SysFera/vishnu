
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
  ${CONFIG_SOURCE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${TMS_SERVER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${TMS_UTILS_DIR}
  ${TORQUE_INCLUDE_DIR}
  ${UTILS_TORQUE_DIR}
  ${SERVER_SOURCE_DIR}
  ${SED_SOURCE_DIR}
)

add_executable(tmssed
  tmssed
  TMSServer
  internalTMSAPI
)

target_link_libraries(tmssed
  ${DIET_SERVER_LIBRARIES}
  ${Boost_LIBRARIES}
  ${TORQUE_LIB}
  emf4cpp-TMS_Data
  register-vishnu
  exception-vishnu
  database-vishnu
  config-vishnu
  utils-vishnu
  utils-server
  ums-server
  ums-sed
  tms-server
)

install_targets(/sbin/
  tmssed
)

add_executable(test_server
  test_server
)

target_link_libraries(test_server
  ${DIET_SERVER_LIBRARIES}
  ${Boost_LIBRARIES}
  ${TORQUE_LIB}
  emf4cpp-TMS_Data
  register-vishnu
  exception-vishnu
  database-vishnu
  utils-vishnu
  utils-server
  tms-server
)
