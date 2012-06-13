link_directories(${Boost_LIBRARY_DIRS})

include_directories(${Boost_INCLUDE_DIRS}
  ${DIET_INCLUDE_DIR}
  ${EMF_DATA_DIR}
  ${UMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${UMS_API_SOURCE_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UTILVISHNU_SOURCE_DIR})

#declare all command lines provided

set(ALLCLI current_session_id connect reconnect close
  add_user update_user delete_user
  change_password reset_password
  add_local_account update_local_account delete_local_account
  save_configuration restore_configuration
  add_machine update_machine delete_machine
  configure_default_option configure_option
  list_local_accounts list_machines list_history_cmd list_options
  list_users list_sessions)

#declare all scripting commands
set(SCRIPTCLI setting_ssh_key)

#set the prefix of command
set (CMDPREFIX vishnu_)

# macro to generate all command lines
macro(genCli NAME )

  set(source_files ${NAME})
  set(CMDNAME ${CMDPREFIX}${NAME})
  #Configuration de l'executable
  add_executable (${CMDNAME}
                  ${source_files})

  #Configuration de l'edition de liens
  target_link_libraries(${CMDNAME}
    ${Boost_LIBRARIES}
    ${VISHNU_EXCEPTION_LIB}
    vishnu-ums-client
    )
  install(TARGETS ${CMDNAME} DESTINATION ${BIN_INSTALL_DIR})
endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})
  genCli(${cli})
endforeach()

foreach(script ${SCRIPTCLI})
   set(SCRIPTNAME "${CMDPREFIX}${script}")
   configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${script}.in" ${PROJECT_BINARY_DIR}/bin/${SCRIPTNAME} COPYONLY)
   install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/${SCRIPTNAME} DESTINATION ${BIN_INSTALL_DIR})
endforeach()


