
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
  ${CONFIG_SOURCE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${FMS_SERVER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${FMS_UTILS_DIR}
  ${SERVER_SOURCE_DIR}
  ${SED_SOURCE_DIR}
)

add_executable(fmssed
  fmssed
  ServerFMS
  internalApi
  getInfosSrv
  getContentSrv
  headSrv
  tailSrv
  mkdirSrv
  mkfileSrv
  rmdirSrv
  rmfileSrv
  chgrpSrv
  chmodSrv
  lsSrv
  transferSrv
  MonitorFMS
  )

target_link_libraries(fmssed
  ${DIET_SERVER_LIBRARIES}
  ${DIET_CLIENT_LIBRARIES}
  ${Boost_LIBRARIES}
  emf4cpp-FMS_Data
  register-vishnu
  exception-vishnu
  database-vishnu
  config-vishnu
  utils-vishnu
  utils-server
  ums-server
  fms-server
)

install(TARGETS fmssed DESTINATION sbin)

