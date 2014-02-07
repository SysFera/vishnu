include_directories(
  ${CONFIG_SOURCE_DIR}
  ${ZMQ_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_SOURCE_DIR}/communication/
  ${VISHNU_SOURCE_DIR}/communication/dispatcher
  ${VISHNU_SOURCE_DIR}/TMS/src/sed
  ${VISHNU_SOURCE_DIR}/UMS/src/sed
  ${VISHNU_SOURCE_DIR}/FMS/src/sed
  ${VISHNU_SOURCE_DIR}/IMS/src/sed
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}

)


# --------------------------------------------------------------------------
#add_subdirectory(ZMQHelper)
# --------------------------------------------------------------------------

add_executable(pinger pinger.cpp)
  
target_link_libraries(pinger
  ${ZMQ_LIBRARIES} 
  ${Boost_LIBRARIES}
  zmq_helper
  vishnu-core
  ${OPENSSL_LIBRARIES}
  )

add_executable(allPinger allPinger.cpp)
  
target_link_libraries(allPinger
  ${ZMQ_LIBRARIES} 
  ${Boost_LIBRARIES}
  zmq_helper
  vishnu-core
  ${OPENSSL_LIBRARIES}
  )

