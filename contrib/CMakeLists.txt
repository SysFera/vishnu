if (INSTALL_SYSV AND NOT CLIENT_ONLY)
  # hack to avoid installing configuration stuff in /usr/etc
  if(${CMAKE_INSTALL_PREFIX} MATCHES "^/usr/?$")
    set(ETC_INSTALL_DIR "/etc")
  else()
    set(ETC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/etc")
  endif()
  set(SBIN_DIR ${CMAKE_INSTALL_PREFIX}/${SBIN_INSTALL_DIR})

  set(CONFIG_FILES SeD.cfg UMS.cfg)
  set(SYSV_FILES vishnu-ums)
  if (COMPILE_TMS)
    set(CONFIG_FILES TMS.cfg)
    set(SYS_FILES vishnu-tms)
  endif()

  if (COMPILE_FMS)
    set(CONFIG_FILES FMS.cfg)
    set(SYS_FILES vishnu-fms)
  endif()

  foreach(CONFIG_FILE ${CONFIG_FILES})
    configure_file("${CONFIG_FILE}.in"  ${CONFIG_FILE})
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE}
    DESTINATION ${ETC_INSTALL_DIR})
  endforeach()

  foreach(SYSV_FILE ${SYSV_FILES})
    configure_file("${SYSV_FILE}.in" ${SYSV_FILE})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${SYSV_FILE}
      DESTINATION "${ETC_INSTALL_DIR}/init.d/"
      PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
  endforeach()
endif()