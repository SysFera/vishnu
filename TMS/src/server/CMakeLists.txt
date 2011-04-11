
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
  ${DATA_BASE_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${SED_SOURCE_DIR}
  ${TMS_UTILS_DIR}
  ${TORQUE_INCLUDE_DIR}
  ${UTILS_TORQUE_DIR}
  ${SERVER_SOURCE_DIR}
)

add_library(tms-server STATIC
   BatchServer
   TorqueServer
   SSHJobExec
   JobServer
)

target_link_libraries(tms-server
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${Boost_LIBRARIES}
  ${LIBCRYPT_LIB}
  ${TORQUE_LIB}
  emf4cpp-TMS_Data
  database-vishnu
  exception-vishnu
  register-vishnu
  ums-server
  utils-vishnu
  utils-server
  utils-torque
)
