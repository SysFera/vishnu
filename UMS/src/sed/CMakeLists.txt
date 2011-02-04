
include_directories(
  ${DIET_DIR}/include
  ${DIET_INCLUDE_DIR}
  ${DIET_DIR}/include
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_CPP_DIR}/include/emf4cpp
  ${EMF4CPP_INCLUDE_DIR} 
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${POSTGRESQL_INCLUDE_DIR}
  ${DATA_BASE_INCLUDE_DIR}
  ${SERVER_SOURCE_DIR}
)

add_executable(
 main main.cc ServerUMS.cc ServerUMS.hh
)

target_link_libraries(main 
  ${DIET_SED_LIB}
  ${LIBPQ_LIB}
  ${EMF4CPP_UMS_DATA_LIB}
  ${VISHNU_EXCEPTION_LIB}
  ${DATA_BASE_LIB}
  #exception-vishnu
  #database-vishnu
  umsserver
)






