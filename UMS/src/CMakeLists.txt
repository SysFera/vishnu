if (NOT CLIENT_ONLY)
  add_subdirectory(sed)
  add_subdirectory(server)
endif (NOT CLIENT_ONLY)  

add_subdirectory(utils/emfdata)
add_subdirectory(api)
add_subdirectory(client)
add_subdirectory(cli)
