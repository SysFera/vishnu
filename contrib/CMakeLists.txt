if(COMPILE_SERVERS)
  # common stuff for init scripts
  # hack to avoid installing configuration stuff in /usr/etc
  if(${CMAKE_INSTALL_PREFIX} MATCHES "^/usr/?$")
    set(ETC_INSTALL_DIR "/etc")
  else()
    set(ETC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/etc")
  endif()
  set(SBIN_DIR ${CMAKE_INSTALL_PREFIX}/${SBIN_INSTALL_DIR})
  # basename for init scriptsx
  set(INIT_FILES init.d/debian/vishnu-dispatcher
                 init.d/debian/vishnu-umssed)
  if (COMPILE_TMS)
    set(INIT_FILES ${INIT_FILES} init.d/debian/vishnu-tmssed)
  endif()
  if(COMPILE_FMS)
    set(INIT_FILES ${INIT_FILES} init.d/debian/vishnu-fmssed)
  endif()

  # install sysV init scripts
  if(INSTALL_SYSV)
    foreach(INIT_FILE ${INIT_FILES})
      configure_file("${INIT_FILE}.in" ${INIT_FILE})
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${INIT_FILE}
        DESTINATION "${ETC_INSTALL_DIR}/init.d/"
        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
    endforeach()
  endif()

  # install systemd services
  if(INSTALL_SYSTEMD)
    foreach(INIT_FILE ${INIT_FILES})
      configure_file("${INIT_FILE}-sysd.in" "${INIT_FILE}.service")
      install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${INIT_FILE}.service"
        DESTINATION "/lib/systemd/system/"
        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
    endforeach()
  endif()
endif(COMPILE_SERVERS)
