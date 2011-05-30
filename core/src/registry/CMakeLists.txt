INCLUDE_DIRECTORIES(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${UTILVISHNU_SOURCE_DIR}
 ${CONFIG_SOURCE_DIR}
 ${EMF_DATA_DIR}
 ${UMS_DATA_DIR}
 ${TMS_EMF_DATA_DIR}
 ${TMS_DATA_DIR}
 ${TMS_UTILS_DIR}
 ${EMF4CPP_INCLUDE_DIR}
 ${SERVER_SOURCE_DIR}
 ${POSTGRESQL_INCLUDE_DIR}
 ${DATA_BASE_INCLUDE_DIR}
 ${SERVER_SOURCE_DIR}
 ${Boost_INCLUDE_DIRS}
)

ADD_LIBRARY(register-vishnu STATIC
  MapperRegistry.cpp
  Mapper.cpp
  UMSMapper.cpp
  FMSMapper.cpp
  IMSMapper.cpp
  TMSMapper.cpp
  )

# Register is linked to all VISHNU modules due to serialized storage
# of command parameters that are used to display the command history
target_link_libraries(register-vishnu
  emf4cpp-UMS_Data
  emf4cpp-TMS_Data
  emf4cpp-IMS_Data
  emf4cpp-FMS_Data
)

SET_TARGET_PROPERTIES(register-vishnu PROPERTIES COMPILE_FLAGS "-fPIC")
