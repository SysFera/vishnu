include_directories(
  ../exception
  ../utils
  ../database
)

# internal configuration parsers
set( config_SRCS
#   CmdParser.cc
  CommonParser.cpp
#   EnvParser.cc
  FileParser.cpp
  constants.cpp
  ExecConfiguration.cpp
  DbConfiguration.cpp
)

# static because we don't ship this
add_library( config-vishnu STATIC ${config_SRCS} )
SET_TARGET_PROPERTIES( config-vishnu PROPERTIES COMPILE_FLAGS "-fPIC")