include_directories(
    ${VISHNU_SOURCE_DIR}/UMS/src/client
)

include(UnitTest)
unit_test(utilsClientUnitTests vishnu-ums-client)

if(COMPILE_CLIENT_CLI AND COMPILE_SERVERS)

  include_directories(
    ${CONFIG_SOURCE_DIR}
    ${DATA_BASE_INCLUDE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${REGISTRY_SOURCE_DIR}
    ${AUTHENTICATOR_INCLUDE_DIR})


  set(server_SRCS_mock ${VISHNU_SOURCE_DIR}/UMS/src/server/SessionServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/UserServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/MachineClientServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/OptionValueServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/MachineServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/LocalAccountServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/ConfigurationServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/CommandServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/AuthSystemServer.cpp
    ${VISHNU_SOURCE_DIR}/UMS/src/server/AuthAccountServer.cpp
    )

  add_library(vishnu-ums-server-mock ${server_SRCS_mock})
  set_target_properties(vishnu-ums-server-mock PROPERTIES VERSION ${VISHNU_VERSION})

  target_link_libraries(vishnu-ums-server-mock vishnu-core vishnu-core-server-mock ${CMAKE_DL_LIBS})
endif()
