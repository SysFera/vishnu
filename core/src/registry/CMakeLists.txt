#SET( CMAKE_INSTALL_PREFIX ../../.. )

INCLUDE_DIRECTORIES(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${UTILVISHNU_SOURCE_DIR}
 ${EMF_DATA_DIR}
 ${UMS_DATA_DIR}
 ${EMF4CPP_INCLUDE_DIR}
 ${SERVER_SOURCE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${SERVER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${REGISTRY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
)

LINK_DIRECTORIES(
#  ../../../lib
)

ADD_EXECUTABLE(mainReg
  main.cpp
  Mapper_impl.cpp
  Mapper_impl2.cpp
  )

ADD_LIBRARY(register-vishnu
  MapperRegistry.cpp
  Mapper.cpp
  UMSMapper.cpp
  FMSMapper.cpp
  IMSMapper.cpp
  TMSMapper.cpp
  )

TARGET_LINK_LIBRARIES(mainReg 
  register-vishnu
  exception-vishnu
  utils-vishnu 
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  ${EMF4CPP_LIB}
  ${EMF4CPP_CPP_LIB}
  emf4cpp-UMS_Data
)

install( TARGETS register-vishnu DESTINATION lib )
