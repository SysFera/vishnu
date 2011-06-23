
link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${Boost_LIBRARY_DIRS}
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${IMS_EMF_DATA_DIR}
  ${EMF_DATA_DIR}
  ${IMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${SERVER_SOURCE_DIR}
  ${IMS_SERVER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${OMNIORB4_INCLUDE_DIR}
  ${LOG_INCLUDE_DIR}
)

add_executable(imssed
  imssed.cpp
  ServerIMS.cpp
  internalApiIMS.cpp
)

target_link_libraries(imssed
  ${DIET_SERVER_LIBRARIES}
  ${Boost_LIBRARIES}
  ums-server
  register-vishnu
  exception-vishnu
  database-vishnu
  config-vishnu
  ims-server
)

install(TARGETS imssed DESTINATION sbin)

