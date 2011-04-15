
link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
 ${Boost_LIBRARY_DIRS}
)

if (VISHNU_USE_TORQUE AND VISHNU_USE_LOADLEVELER)
  set(UsedBatchServer TorqueServer LLServer)
  set(USED_BATCH_INCLUDE_DIR ${TORQUE_INCLUDE_DIR} ${UTILS_TORQUE_DIR} ${LOADLEVELER_INCLUDE_DIR})
  set(USED_BATCH_LIB ${TORQUE_LIB} ${LOADLEVELER_LIB}) 
else (VISHNU_USE_TORQUE AND VISHNU_USE_LOADLEVELER)
  if (VISHNU_USE_TORQUE)
    set(UsedBatchServer TorqueServer)
    set(USED_BATCH_INCLUDE_DIR ${TORQUE_INCLUDE_DIR} ${UTILS_TORQUE_DIR})
    set(USED_BATCH_LIB ${TORQUE_LIB})
  else (VISHNU_USE_TORQUE)
    if(VISHNU_USE_LOADLEVELER)
      set(UsedBatchServer LLServer)
      set(USED_BATCH_INCLUDE_DIR ${LOADLEVELER_INCLUDE_DIR})
      set(USED_BATCH_LIB ${LOADLEVELER_LIB})
    else (VISHNU_USE_LOADLEVELER) 
      message("TORQUE or LOADLEVELER  installation was not found. Define the TORQUE_DIR or LOADLEVELER_DIR variable to continue.")
      message(" - You can define a TORQUE_DIR or LOADLEVELER_DIR environment variable")
      message(" - You can pass it as argument to cmake:")
      message("   $ cmake <source root directory> -DTORQUE_DIR:PATH=/path/to/TORQUE or -DLOADLEVELER_DIR:PATH=/path/to/LOADLEVELER")
      message(" - You can use the ccmake GUI")
    endif (VISHNU_USE_LOADLEVELER)
  endif (VISHNU_USE_TORQUE) 
endif (VISHNU_USE_TORQUE AND VISHNU_USE_LOADLEVELER)

include_directories(
  ${DIET_INCLUDE_DIR}
  ${TMS_EMF_DATA_DIR}
  ${TMS_DATA_DIR}
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
  ${SED_SOURCE_DIR}
  ${TMS_SED_SOURCE_DIR}
  ${TMS_UTILS_DIR}
  ${SERVER_SOURCE_DIR}
  ${USED_BATCH_INCLUDE_DIR}
)

add_library(tms-server STATIC
   BatchServer
   ${UsedBatchServer}
   SSHJobExec
   JobServer
   BatchFactory
)

target_link_libraries(tms-server
  ${DIET_CLIENT_LIB}
  ${DIET_SERVER_LIB}
  ${Boost_LIBRARIES}
  ${LIBCRYPT_LIB}
  ${USED_BATCH_LIB}
  emf4cpp-TMS_Data
  database-vishnu
  exception-vishnu
  register-vishnu
  config-vishnu
  ums-server
  utils-vishnu
  utils-server
  utils-torque
)
