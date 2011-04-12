if (NOT CLIENT_ONLY)
  add_subdirectory( database )
endif (NOT CLIENT_ONLY)

add_subdirectory( exception )
add_subdirectory( registry  )
add_subdirectory( utils)
add_subdirectory( config )