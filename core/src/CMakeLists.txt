include_directories(${Boost_INCLUDE_DIRS} config exception utils emfdata
  ../deps/emf4cpp ${UMS_SERVER_SOURCE_DIR} ${TMS_SERVER_SOURCE_DIR}
  ${IMS_SERVER_SOURCE_DIR} ${FMS_SERVER_SOURCE_DIR}
  )

#################### config ###################################################
set(config_SRCS config/CommonParser.cpp
  config/FileParser.cpp
  config/constants.cpp
  config/ExecConfiguration.cpp)

#################### exceptions ###############################################
set(exception_SRCS exception/VishnuException.cpp
  exception/SystemException.cpp
  exception/UserException.cpp
  exception/UMSVishnuException.cpp
  exception/TMSVishnuException.cpp
  exception/IMSVishnuException.cpp
  exception/FMSVishnuException.cpp
  exception/InternalVishnuException.cpp)

set(exception_HEADERS exception/VishnuException.hpp
  exception/UserException.hpp
  exception/SystemException.hpp
  exception/UMSVishnuException.hpp
  exception/TMSVishnuException.hpp
  exception/FMSVishnuException.hpp
  exception/IMSVishnuException.hpp)

#################### utils ####################################################
set(utils_SRCS utils/utilVishnu.cpp
  utils/utilClient.cpp
  utils/Options.cpp
  utils/sessionUtils.cpp
  utils/CLICmd.cpp
  utils/cliUtil.cpp)

#################### register #################################################
set(registry_SRCS registry/MapperRegistry.cpp
  registry/Mapper.cpp
  registry/UMSMapper.cpp
  registry/FMSMapper.cpp
  registry/IMSMapper.cpp
  registry/TMSMapper.cpp)

add_library(vishnu-core ${config_SRCS} ${exception_SRCS}
  ${utils_SRCS} ${registry_SRCS})
install(FILES ${exception_HEADERS} DESTINATION include)

if(NOT CLIENT_ONLY)
  include_directories(database)
  set(database_SRCS 
    database/DbConfiguration.cpp
    database/DbFactory.cpp
    database/Database.cpp
    database/DatabaseResult.cpp)
  set(utils_server_SRCS utils/utilServer.cpp)

  if(MYSQL_FOUND AND ENABLE_MYSQL)
    set(database_SRCS ${database_SRCS} database/MYSQLDatabase.cpp)
    include_directories(${MYSQL_INCLUDE_DIR})
    set(DB_LIBS ${MYSQL_LIB})
  endif()
   
  if(POSTGRESQL_FOUND AND ENABLE_POSTGRESQL)
    set(database_SRCS ${database_SRCS} database/POSTGREDatabase.cpp)
    include_directories(${POSTGRESQL_INCLUDE_DIR})
    set(DB_LIBS ${POSTGRESQL_LIB})
  endif()

  add_library(vishnu-core-server ${database_SRCS} ${utils_server_SRCS})
  target_link_libraries(vishnu-core-server ${DB_LIBS})
endif()