
link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${Boost_LIBRARY_DIRS}
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${FMS_EMF_DATA_DIR}
  ${FMS_DATA_DIR}
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${FMS_UTILS_DIR}
  ${SERVER_SOURCE_DIR}
)

add_library(fms-server STATIC
File
SSHFile
FileFactory
FileTransferCommand
FileTransferServer
ScpCommand
RsyncCommand

)

target_link_libraries(fms-server
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${Boost_LIBRARIES}
  ${LIBCRYPT_LIB}
  emf4cpp-FMS_Data
  database-vishnu
  exception-vishnu
  register-vishnu
  config-vishnu
  ums-server
  utils-vishnu
  utils-server
)
