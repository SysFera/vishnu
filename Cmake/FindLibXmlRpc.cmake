# Find libxmlrpc++ library

# This module defines:
# LIBXMLRPC_FOUND          - whether library was found
# LIBXMLRPC_INCLUDE_DIR    - include path
# LIBXMLRPC_LIBRARIES      - libraries to link against



find_path(LIBXMLRPC_INCLUDE_DIR
  NAMES xmlrpc-c/client.hpp
  PATHS
  ${LIBXMLRPC_DIR}/include
  $ENV{LIBXMLRPC_DIR}/include
  /usr/local/include
  /opt/local/include
  ${LIBXMLRPC_INCLUDE_DIRS}
)
if(NOT LIBXMLRPC_INCLUDE_DIR)
  LIST(APPEND MISSING_HEADERS_LIBS xmlrpc-c/client.hpp)
endif(NOT LIBXMLRPC_INCLUDE_DIR)

find_library(XMLRPC_BASE
  NAMES xmlrpc
  PATHS
  ${LIBXMLRPC_DIR}/lib
  $ENV{LIBXMLRPC_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXMLRPC_LIBRARY_DIRS}
)
if(NOT XMLRPC_BASE)
  LIST(APPEND MISSING_HEADERS_LIBS libxmlrpc)
endif(NOT XMLRPC_BASE)

find_library(XMLRPC_BASE_CPP
  NAMES xmlrpc++
  PATHS
  ${LIBXMLRPC_DIR}/lib
  $ENV{LIBXMLRPC_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXMLRPC_LIBRARY_DIRS})
  
if(NOT XMLRPC_BASE_CPP)
  LIST(APPEND MISSING_HEADERS_LIBS libxmlrpc++)
endif(NOT XMLRPC_BASE_CPP)

find_library(XMLRPC_CLIENT
  NAMES xmlrpc_client
  PATHS
  ${LIBXMLRPC_DIR}/lib
  $ENV{LIBXMLRPC_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXMLRPC_LIBRARY_DIRS})
  
if(NOT XMLRPC_CLIENT)
  LIST(APPEND MISSING_HEADERS_LIBS libxmlrpc_client)
endif(NOT XMLRPC_CLIENT)

find_library(XMLRPC_CLIENT_CPP
  NAMES xmlrpc_client++
  PATHS
  ${LIBXMLRPC_DIR}/lib
  $ENV{LIBXMLRPC_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXMLRPC_LIBRARY_DIRS})
  
if(NOT XMLRPC_CLIENT_CPP)
  LIST(APPEND MISSING_HEADERS_LIBS libxmlrpc_client++)
endif(NOT XMLRPC_CLIENT_CPP)

find_library(XMLRPC_UTIL
  NAMES xmlrpc_util
  PATHS
  ${LIBXMLRPC_DIR}/lib
  $ENV{LIBXMLRPC_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXMLRPC_LIBRARY_DIRS})
  
if(NOT XMLRPC_UTIL)
  LIST(APPEND MISSING_HEADERS_LIBS libxmlrpc_util)
endif(NOT XMLRPC_UTIL)  

find_library(XMLRPC_XMLTOC
  NAMES xmlrpc_xmltok
  PATHS
  ${LIBXMLRPC_DIR}/lib
  $ENV{LIBXMLRPC_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXMLRPC_LIBRARY_DIRS})
  
if(NOT XMLRPC_XMLTOC)
  SET(XMLRPC_XMLTOC "")
endif(NOT XMLRPC_XMLTOC)    

find_library(XMLRPC_XMLPARSE
  NAMES xmlrpc_xmlparse
  PATHS
  ${LIBXMLRPC_DIR}/lib
  $ENV{LIBXMLRPC_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBXMLRPC_LIBRARY_DIRS})   

find_library(XMLRPC_PACKETSOCKET
  NAMES xmlrpc_packetsocket
  PATHS
  ${LIBCURL_DIR}/lib
  $ENV{LIBCURL_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBCURL_LIBRARY_DIRS})
  
if(NOT XMLRPC_PACKETSOCKET)
  LIST(APPEND MISSING_HEADERS_LIBS libxmlrpc_packetsocket)
endif(NOT XMLRPC_PACKETSOCKET)       

  
if(NOT XMLRPC_XMLPARSE)
  set(XMLRPC_XMLPARSE "")
endif(NOT XMLRPC_XMLPARSE)      

find_library(LIBCURL
  NAMES curl
  PATHS
  ${LIBCURL_DIR}/lib
  $ENV{LIBCURL_DIR}/lib
  /usr/local/lib
  /opt/local/lib
  ${LIBCURL_LIBRARY_DIRS})
  
if(NOT LIBCURL)
  LIST(APPEND MISSING_HEADERS_LIBS libcurl)
endif(NOT LIBCURL)  


if (NOT MISSING_HEADERS_LIBS)
  set(LIBXMLRPC_FOUND "Yes")
  set(LIBXMLRPC_LIB ${LIBCURL} 
                    ${XMLRPC_XMLPARSE} 
                    ${XMLRPC_PACKETSOCKET} 
                    ${XMLRPC_XMLPARSE} 
                    ${XMLRPC_XMLTOC} 
                    ${XMLRPC_UTIL} 
                    ${XMLRPC_CLIENT_CPP} 
                    ${XMLRPC_CLIENT} 
                    ${XMLRPC_BASE_CPP} 
                    ${XMLRPC_BASE})
  mark_as_advanced(LIBXMLRPC_INCLUDE_DIR)
  mark_as_advanced(LIBXMLRPC_LIB)
else(NOT MISSING_HEADERS_LIBS) 
  message("missing headers or libs: ${MISSING_HEADERS_LIBS}")
endif(NOT MISSING_HEADERS_LIBS)
