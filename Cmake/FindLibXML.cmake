# Find XML libraries

# This module defines:
# XML_FOUND             - whether libraries were found
# XML_INCLUDE_DIR       - include path
# XML_LIBRARIES         - libraries to link against

find_path(XML_INCLUDE_DIR libxml/xpath.h
  PATHS
  ${XML_DIR}/include
  $ENV{XML_DIR}/include
  /usr/include
  /usr/include/libxml2
  /usr/local/include
  /opt/local/include
)


# LIBS

find_library(XML_LIBRARY
  NAMES xml2
  PATHS ${XML_INSTALL_DIR}/lib
  ${XML_DIR}/lib
  $ENV{XML_DIR}/lib
  /usr/lib
  /usr/lib/i386-linux-gnu/
  /usr/local/lib
  /opt/local/lib
)


if (XML_INCLUDE_DIR AND XML_LIBRARY)
  set(CURL_FOUND "Yes")  
  mark_as_advanced(XML_INCLUDE_DIR)
  mark_as_advanced(XML_LIBRARY)
endif(XML_INCLUDE_DIR AND XML_LIBRARY)


