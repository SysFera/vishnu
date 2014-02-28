include_directories(${Boost_INCLUDE_DIRS}
  ${VISHNU_SOURCE_DIR}/core/src/config
  ${VISHNU_SOURCE_DIR}/core/src/exception
  )

add_library(mockDb
  Database.cpp
  DatabaseResult.cpp
  DbConfiguration.cpp
  DbFactory.cpp
  MockDatabase.cpp
  )

target_link_libraries(mockDb vishnu-core)
if(COMPILE_SERVERS)

  include_directories(${VISHNU_SOURCE_DIR}/core/src/utils
  ${VISHNU_SOURCE_DIR}/core/src/emfdata
  ${VISHNU_SOURCE_DIR}/core/deps/emf4cpp
  ${UMS_SERVER_SOURCE_DIR}
  ${TMS_SERVER_SOURCE_DIR}
  ${FMS_SERVER_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${DATABASE_INCLUDE_DIR}
  ${AUTHENTICATOR_INCLUDE_DIR}
  ${VERSION_MANAGER_SOURCE_DIR}
  ${COMMUNICATION_INCLUDE_DIR}
  ${VISHNU_SOURCE_DIR}/core/test/mock/database/
  )
  set(utils_server_SRCS ${VISHNU_SOURCE_DIR}/core/src/utils/utilServer.cpp
  #    utils/Logger.cpp
      )

  if ((OPENSSL_FOUND) AND (ENABLE_LDAP) AND (LDAP_FOUND))
    # we add compilation variable definitions only on required files
    set_source_files_properties(
      authenticator/AuthenticatorFactory.cpp
      PROPERTIES COMPILE_FLAGS "-DUSE_LDAP"
      )

    include_directories(${LDAP_PROXY_SOURCE_DIR})
    set(authenticator_SRCS
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/AuthenticatorConfiguration.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/AuthenticatorFactory.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/Authenticator.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/UMSAuthenticator.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/LDAPAuthenticator.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/UMSLDAPAuthenticator.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/LDAPUMSAuthenticator.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/ldap/LDAPProxy.cpp)
    set(AUTH_LIBS ${LDAP_LIBRARIES})
  else ((OPENSSL_FOUND) AND (ENABLE_LDAP) AND (LDAP_FOUND))
    set(authenticator_SRCS
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/AuthenticatorConfiguration.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/AuthenticatorFactory.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/Authenticator.cpp
    ${VISHNU_SOURCE_DIR}/core/src/authenticator/UMSAuthenticator.cpp)
  endif ((OPENSSL_FOUND) AND (ENABLE_LDAP) AND (LDAP_FOUND))

  include_directories(${UMS_EMF_DATA_DIR})
  set (version_manager_SRCS
       ${VISHNU_SOURCE_DIR}/core/src/version/VersionManager.cpp)

  add_library(vishnu-core-server-mock ${utils_server_SRCS} ${authenticator_SRCS} ${version_manager_SRCS})
  set_target_properties(vishnu-core-server-mock PROPERTIES VERSION ${VISHNU_VERSION})
  target_link_libraries(vishnu-core-server-mock vishnu-core mockDb ${AUTH_LIBS})
endif()
