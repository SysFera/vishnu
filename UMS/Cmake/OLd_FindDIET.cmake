#
# Find a Diet installation or build tree.
#
# Diet will be considered found when a DietConfig.cmake file is encountered.
# DietConfig.cmake is a CMake source file (generated from DietConfig.cmake.in
# at install time of Diet). This DietConfig.cmake will be sourced to setup
# the location and configuration of Diet i.e. the pathes to Diet include
# directories, library directories, and preprocessor macros... Please read
# this file, or DietConfig.cmake.in from the Diet source tree, for the full
# list of CMake variable definitions. 
#
# In addition to the variables read from DietConfig.cmake, this find
# module also defines:
#
# DIET_USE_FILE - The full (with path) filename of DietConfig.cmake file
#                 that shall be included.
# DIET_DIR      - The directory containing DIETConfig.cmake. This is either
#                 the share sub-directory of the root of a build tree, or
#                 the share sub-directory a Diet's installation directory.
#                 Hence, DIET_DIR + "/share/DietConfig.cmake" = DIET_USE_FILE
# DIET_FOUND    - Whether DIET was found. If this is true, DIET_DIR is okay.
#

SET( DIET_DIR_STRING "directory containing the file share/DietConfig.cmake. This is either the root of the build tree, or PREFIX of an installation." )
SET( DIET_FOUND_STRING "Whether a Diet installation was found." )

IF( NOT DIET_DIR )
  ##### An indicative DIET_DIR was not provided and we search the well
  # known system path that we use as search path:
  IF(UNIX)
    STRING(REGEX MATCHALL "[^:]+" DIET_DIR_SEARCH1 "$ENV{PATH}")
  ELSE(UNIX)
    STRING(REGEX REPLACE "\\\\" "/" DIET_DIR_SEARCH1 "$ENV{PATH}")
  ENDIF(UNIX)
  STRING(REGEX REPLACE "/;" ";" DIET_DIR_SEARCH2 ${DIET_DIR_SEARCH1})

  ##### Construct a set of paths relative to the system search path.
  SET( DIET_DIR_SEARCH "" )
  FOREACH( dir ${DIET_DIR_SEARCH2} )
    SET( DIET_DIR_SEARCH ${DIET_DIR_SEARCH} "${dir}/../share" )
  ENDFOREACH( dir )

  ##### Look for an installation or build tree.
  FIND_PATH( DIET_DIR share/DietConfig.cmake
    # Look for an environment variable DIET_DIR.
    $ENV{DIET_DIR}

    # Look in places relative to the system executable search path.
    ${DIET_DIR_SEARCH}

    # Look in standard UNIX install locations.
    /usr/local

    # Help the user find it if we cannot.
    DOC "The ${DIET_DIR_STRING}"
  )
  
  IF( DIET_DIR )
    SET( DIET_FOUND TRUE CACHE BOOL ${DIET_FOUND_STRING} FORCE )
  ELSE( DIET_DIR )
    SET( DIET_FOUND FALSE CACHE BOOL ${DIET_FOUND_STRING} FORCE )
  ENDIF( DIET_DIR )

ELSE( NOT DIET_DIR )
  ##### An indicative DIET_DIR was provided: check it
  IF( EXISTS ${DIET_DIR}/share/DietConfig.cmake )
    SET( DIET_FOUND TRUE CACHE BOOL ${DIET_FOUND_STRING} FORCE )
  ELSE( EXISTS ${DIET_DIR}/share/DietConfig.cmake )
    SET( DIET_FOUND FALSE CACHE BOOL ${DIET_FOUND_STRING} FORCE )
  ENDIF( EXISTS ${DIET_DIR}/share/DietConfig.cmake )
ENDIF( NOT DIET_DIR )

MESSAGE("XXXXXXXXXXXXXXXXXXX ${DIET_DIR}")

IF( DIET_FOUND )
  #MESSAGE("YYYYYYYYYYYYYYYYYYYYYYYYYY")
  MARK_AS_ADVANCED( DIET_FOUND )
  SET( DIET_USE_FILE ${DIET_DIR}/share/DietConfig.cmake
       CACHE STRING "Full (with path) filename of DietConfig.cmake."  )
  MARK_AS_ADVANCED( DIET_USE_FILE )
  # Load the file to check if everything is fine:
  INCLUDE( ${DIET_USE_FILE} )
ENDIF( DIET_FOUND )
