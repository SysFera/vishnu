
link_directories(${Boost_LIBRARY_DIRS}
                  ${IMS_CLI_SOURCE_DIR}
                  ${CLI_SOURCE_DIR}
		  ${LIBRARY_OUTPUT_PATH}
		  ${IMS_CLIENT_PROXY_SOURCE_DIR}
                )

include_directories(${Boost_INCLUDE_DIRS}
                    ${DIET_INCLUDE_DIR}
                    ${IMS_EMF_DATA_DIR}
                    ${EMF4CPP_INCLUDE_DIR}
                    ${IMS_API_SOURCE_DIR}
                    ${IMS_CLIENT_PROXY_SOURCE_DIR}
                    ${VISHNU_EXCEPTION_INCLUDE_DIR}
                    ${IMS_CLI_SOURCE_DIR}
                    ${UTILVISHNU_SOURCE_DIR}
		    ${API_SOURCE_DIR}
		    ${UMS_CLIENT_PROXY_SOURCE_DIR}
		    ${EMF_DATA_DIR}
		    ${UMS_EMF_DATA_DIR}
		    ${IMS_UTIL_DIR}
                  )


#declare all command lines provided

set(ALLCLI
  get_processes
  get_metric_history
  get_update_frequency
  set_update_frequency
  get_metric_current_value
  set_system_info
  get_system_info
  define_user_identifier
  define_machine_identifier
  define_transfer_identifier
  define_job_identifier
  define_auth_identifier
  set_system_threshold
  get_system_threshold
  restart
  stop
  load_shed
  export_commands)

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
                        ${LOG_LIBRARY}
                        ${Boost_LIBRARIES}
                        vishnu-ums-client
                        vishnu-ims-client
                        vishnu-core)
  install(TARGETS ${CMDNAME} DESTINATION ${BIN_INSTALL_DIR})

endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})
  genCli(${cli})
endforeach(cli)

