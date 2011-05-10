
include_directories(
  ${TORQUE_INCLUDE_DIR}
)


if (VISHNU_USE_TORQUE)

  set (PBS_SERVER_HOME "/var/spool/torque" CACHE STRING "The torque server directory")
  mark_as_advanced(PBS_SERVER_HOME)

  if (EXISTS ${PBS_SERVER_HOME})
  else (EXISTS ${PBS_SERVER_HOME})
    message("The server home/spool directory for PBS installation was not found. Define the PBS_SERVER_HOME variable to continue.")
    message(" - You can define a PBS_SERVER_HOME environment variable")
    message(" - You can pass it as argument to cmake:")
    message("   $ cmake <source root directory> -DPBS_SERVER_HOME:PATH=/path/to/pbs_server_home")
    message(" - You can use the ccmake GUI")
    set(PBS_SERVER_HOME "" cache path "DIET installation path") 
  endif(EXISTS ${PBS_SERVER_HOME})

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pbs_server_config.h.in
                 ${CMAKE_CURRENT_SOURCE_DIR}/pbs_server_config.h)

  add_library(utils-torque STATIC
    pbs_sub
    )

  target_link_libraries(utils-torque
    ${TORQUE_LIB}
    c
    )
endif (VISHNU_USE_TORQUE)
