
link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${Boost_LIBRARY_DIRS}
)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${CONFIG_SOURCE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${UMS_SERVER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
)

add_executable(umssed
  umssed.cpp
  ServerUMS.cpp
  internalApi.cpp
  MonitorUMS.cpp
)

target_link_libraries(umssed
  ${DIET_SERVER_LIBRARIES}
  ${Boost_LIBRARIES}
  exception-vishnu
  ums-server
  vishnu-core
  vishnu-core-server
)

install(TARGETS umssed DESTINATION sbin)

