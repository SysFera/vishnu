# To find if the diet log service module is installed
# And to set some environemment variables
#
# Variables set :
# LOG_LIB_DIR: Directory containing all the lib
# LOG_INCLUDE_DIR: Directory containing the files to include

FILE( APPEND ${CMAKE_BINARY_DIR}/CMakeOutput.log
      "FindLOGSERVICE.cmake: Looking for Log Service library.\n\n" )

set (libName "LogForwarderUtils")

find_path(LOG_INCLUDE_DIR LogORBMgr.hh
  PATHS
    ${LOGSERVICE_DIR}/include
    ${DIET_DIR}/include
    /usr/local/include
    /opt/local/include
    /usr/include
  DOC "Directory containing the log service include files")

find_library(LOG_LIBRARY ${libName}
  PATHS
    ${LOGSERVICE_DIR}/lib
    ${DIET_DIR}/lib
    /usr/local/lib
    /opt/local/lib
    /usr/lib
  DOC "The log service library")

find_program(LOG_CENTRAL LogCentral
  PATHS
    ${LOGSERVICE_DIR}/bin
    ${DIET_DIR}/bin
    /usr/local/bin
    /opt/local/bin
    /usr/bin
  DOC "The log service binary")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LOGSERVICE DEFAULT_MSG LOG_INCLUDE_DIR LOG_LIBRARY)

set (LOGSERVICE_FOUND "NO")

if (LOG_INCLUDE_DIR)
if (LOG_LIBRARY)
if (LOG_CENTRAL)
   SET(LOGSERVICE_FOUND  "YES" )
   MARK_AS_ADVANCED (LOG_LIBRARY)
   MARK_AS_ADVANCED (LOG_INCLUDE_DIR)
   MARK_AS_ADVANCED (LOG_CENTRAL)
endif (LOG_CENTRAL)
endif (LOG_LIBRARY)
endif (LOG_INCLUDE_DIR)

if (NOT LOGSERVICE_FOUND)
  MESSAGE("LogService not found on this machine. DIET will not be able to use the log system.")
  SET(LOGSERVICE_DIR "" CACHE PATH "Root of log service tree installation".)
endif (NOT LOGSERVICE_FOUND)

