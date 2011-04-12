include_directories(
  ../exception
  ../utils
)

# internal configuration parsers
set( config_SRCS
#   CmdParser.cc
  CommonParser.cpp
#   EnvParser.cc
  FileParser.cpp
  constants.cpp
  configuration.cpp
)

# static because we don't ship this
add_library( config-vishnu STATIC ${config_SRCS} )