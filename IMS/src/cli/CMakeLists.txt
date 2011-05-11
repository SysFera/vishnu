
link_directories(${Boost_LIBRARY_DIRS}
                  ${IMS_CLI_SOURCE_DIR}
                  ${CLI_SOURCE_DIR}
		  ${LIBRARY_OUTPUT_PATH}
		  ${IMS_CLIENT_PROXY_SOURCE_DIR}
                )

include_directories(${Boost_INCLUDE_DIRS}
                    ${DIET_INCLUDE_DIR}
                    ${IMS_EMF_DATA_DIR}
                    ${IMS_DATA_DIR}
                    ${EMF4CPP_INCLUDE_DIR}
                    ${IMS_API_SOURCE_DIR}
                    ${IMS_CLIENT_PROXY_SOURCE_DIR}
                    ${VISHNU_EXCEPTION_INCLUDE_DIR}
                    ${IMS_CLI_SOURCE_DIR}
                    ${UTILVISHNU_SOURCE_DIR}
		    ${API_SOURCE_DIR}
		    ${CLIENT_PROXY_SOURCE_DIR}
		    ${EMF_DATA_DIR}
		    ${UMS_DATA_DIR}
		    ${IMS_UTIL_DIR}
                  )

add_library(ims-utils-cli
	    displayer
 )
#declare all command lines provided

set(ALLCLI
  get_processes
  get_history_metric
  get_update_frequency
  set_update_frequency
  get_current_metric
  set_system_info
  get_system_info
  define_user_format
  define_machine_format
  define_file_format
  define_task_format
  set_system_threshold
  get_system_threshold
  restart
  )

#set the prefix of command
set (CMDPREFIX vishnu_)

# macro to generate all command lines

macro(genCli NAME )

  set(source_files ${NAME}
      )

  set(CMDNAME ${CMDPREFIX}${NAME})

  #Configuration de l'executable
  add_executable (${CMDNAME}
                  ${source_files}
                  )

  #Configuration de l'edition de liens
  target_link_libraries(${CMDNAME}
                        ${Boost_LIBRARIES}
                        ${VISHNU_EXCEPTION_LIB}
                        ims-utils-cli
                        utils-vishnu
                        vishnu-ims-api
                        vishnu-ums-api
			ims-client-proxy
                        )
  install_targets(/bin/
                  ${CMDNAME}
                )

endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})

genCli(${cli})

endforeach(cli)

install( TARGETS ims-utils-cli DESTINATION lib )

