# internal configuration parsers
set( config_SRCS
#   CmdParser.cc
  CommonParser.cpp
#   EnvParser.cc
  FileParser.cpp
#   constants.cc
#   configuration.cc
)

# static because we don't ship this
add_library( VishnuConfig STATIC ${config_SRCS} )