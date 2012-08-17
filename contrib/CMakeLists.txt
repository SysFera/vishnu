if(NOT CLIENT_ONLY)
  # common stuff for init scripts
  # hack to avoid installing configuration stuff in /usr/etc
  if(${CMAKE_INSTALL_PREFIX} MATCHES "^/usr/?$")
    set(ETC_INSTALL_DIR "/etc")
  else()
    set(ETC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/etc")
  endif()
  set(SBIN_DIR ${CMAKE_INSTALL_PREFIX}/${SBIN_INSTALL_DIR})
  # configuration files for vishnu services
  set(CONFIG_FILES SeD.cfg UMS.cfg)
  if(COMPILE_FMS)
    set(CONFIG_FILES ${CONFIG_FILES} FMS.cfg)
  endif()
  if(COMPILE_TMS)
    set(CONFIG_FILES ${CONFIG_FILES} TMS.cfg)
  endif()
  if(COMPILE_IMS)
    set(CONFIG_FILES ${CONFIG_FILES} IMS.cfg)
  endif()
  # basename for init scriptsx
  set(INITx_FILES vishnu-ums)
  if (COMPILE_TMS)
    set(INIT_FILES ${INIT_FILES} vishnu-tms)
  endif()
  if(COMPILE_FMS)
    set(INIT_FILES ${INIT_FILES} vishnu-fms)
  endif()
  if(COMPILE_TMS)
    set(INIT_FILES ${INIT_FILES} vishnu-tms)
  endif()
  if(COMPILE_IMS)
    set(INIT_FILES ${INIT_FILES} vishnu-ims)
  endif()

  if(INSTALL_SYSV OR INSTALL_SYSTEMD)
    # install configuration files common to both init
    foreach(CONFIG_FILE ${CONFIG_FILES})
      configure_file("${CONFIG_FILE}.in"  ${CONFIG_FILE})
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE}
        DESTINATION ${ETC_INSTALL_DIR}/vishnu)
    endforeach()
  endif()

  # install sysV init scripts
  if(INSTALL_SYSV)
    foreach(INIT_FILE ${SYSV_FILES})
      configure_file("${INIT_FILE}.init.in" ${INIT_FILE})
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${INIT_FILE}
        DESTINATION "${ETC_INSTALL_DIR}/init.d/"
        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
    endforeach()
  endif()

  # install systemd services
  if(INSTALL_SYSTEMD)
    foreach(INIT_FILE ${INIT_FILES})
      configure_file("${INIT_FILE}.service.in" "${INIT_FILE}.service")
      install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${INIT_FILE}.service"
        DESTINATION "/lib/systemd/system/"
        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
    endforeach()
  endif()
endif()
