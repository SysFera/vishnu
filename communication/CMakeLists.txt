if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

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
  ${PROJECT_BINARY_DIR}
  ${LIBJANSSON_INCLUDE_DIR}
)

if(NOT COMPILE_ONLY_LIBBATCH)
  add_library(zmq_helper
    DIET_client.cpp
    Annuary.cpp
    Server.cpp
    SeD.cpp
    utils.cpp
    sslhelpers.cpp
    ${utils_server_SRCS}
  )

  target_link_libraries (zmq_helper
    ${ZMQ_LIBRARIES}
    ${Boost_LIBRARIES}
    vishnu-core
    ${LIBCRYPTO_LIB}
    ${OPENSSL_LIBRARIES}
    ${LIBJANSSON_LIB}
  )
  set_target_properties(zmq_helper PROPERTIES VERSION ${VISHNU_VERSION})
  install(TARGETS zmq_helper DESTINATION ${LIB_INSTALL_DIR})
endif(NOT COMPILE_ONLY_LIBBATCH)

# --------------------------------------------------------------------------
#add_subdirectory(ZMQHelper)
# --------------------------------------------------------------------------

if(COMPILE_DISPATCHER)
  add_executable(dispatcher dispatcher/main.cpp
    dispatcher/Dispatcher.cpp)
  
  target_link_libraries(dispatcher
    ${ZMQ_LIBRARIES} 
    ${Boost_LIBRARIES}
    zmq_helper
    vishnu-core
    ${OPENSSL_LIBRARIES}
    )
  install(TARGETS dispatcher DESTINATION ${SBIN_INSTALL_DIR})
endif(COMPILE_DISPATCHER)

add_subdirectory(pinger)
