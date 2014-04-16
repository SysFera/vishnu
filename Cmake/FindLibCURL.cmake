# Find CURL libraries

# This module defines:
# CURL_FOUND             - whether libraries were found
# CURL_INCLUDE_DIR       - include path
# CURL_LIBRARIES         - libraries to link against

find_path(CURL_INCLUDE_DIR curl/curl.h
  PATHS
  ${CURL_DIR}/include
  $ENV{CURL_DIR}/include
  /usr/include
  /usr/local/include
  /opt/local/include
)


# LIBS

find_library(CURL_LIBRARY
  NAMES curl
  PATHS ${CURL_INSTALL_DIR}/lib
  ${CURL_DIR}/lib
  $ENV{CURL_DIR}/lib
  /usr/lib
  /usr/lib/i386-linux-gnu
  /usr/lib/x64_86-linux-gnu
  /usr/local/lib
  /opt/local/lib
)


if (CURL_INCLUDE_DIR AND CURL_LIBRARY)
  set(CURL_FOUND "Yes")  
  mark_as_advanced(CURL_INCLUDE_DIR)
  mark_as_advanced(CURL_LIBRARY)
endif(CURL_INCLUDE_DIR AND CURL_LIBRARY)


