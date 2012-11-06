# Find XMLRPC libraries

# This module defines:
# XMLRPC_FOUND             - whether libraries were found
# XMLRPC_INCLUDE_DIR       - include path
# XMLRPC_LIBRARIES         - libraries to link against

find_path(XMLRPC_INCLUDE_DIR xmlrpc.h
  PATHS
  ${XMLRPC_DIR}/include
  $ENV{XMLRPC_DIR}/include
  /usr/include
  /usr/local/include
  /opt/local/include
)


# LIBS

find_library(XMLRPC_CLIENTPP_LIBRARY
  NAMES xmlrpc_client++
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(XMLRPCPP_LIBRARY
  NAMES xmlrpc++
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(XMLRPC_PACKSO_LIBRARY
  NAMES xmlrpc_packetsocket
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(XMLRPC_LIBRARY
  NAMES xmlrpc
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(XMLRPC_CLIENT_LIBRARY
  NAMES xmlrpc_client
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(XMLRPC_PARSE_LIBRARY
  NAMES xmlrpc_parse
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(XMLRPC_XMLTOK_LIBRARY
  NAMES xmlrpc_xmltok
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

find_library(XMLRPC_UTIL_LIBRARY
  NAMES xmlrpc_util
  PATHS ${XMLRPC_INSTALL_DIR}/lib
  ${XMLRPC_DIR}/lib
  $ENV{XMLRPC_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

#find_library(LBER_LIBRARY
#  NAMES lber
#  PATHS ${XMLRPC_INSTALL_DIR}/lib)

set(XMLRPC_LIBRARIES 
  ${XMLRPC_CLIENTPP_LIBRARY}
  ${XMLRPCPP_LIBRARY}
  ${XMLRPC_PARSE_LIBRARY}
  ${XMLRPC_XMLTOK_LIBRARY}
  ${XMLRPC_UTIL_LIBRARY}
  ${XMLRPC_PACKSO_LIBRARY}
  ${XMLRPC_CLIENT_LIBRARY}
  ${XMLRPC_LIBRARY}
  )

# defines our variable
#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(XMLRPC DEFAULT_MSG
#  XMLRPC_LIBRARIES XMLRPC_LIBRARY LBER_LIBRARY XMLRPC_INCLUDE_DIR)


