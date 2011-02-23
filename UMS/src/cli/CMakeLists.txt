
link_directories(${Boost_LIBRARY_DIRS}
                  ${EMF4CPP_LIB}
                  ${EMF4CPP_CPP_LIB}
                  ${CLI_SOURCE_DIR}
                )
include_directories(${Boost_INCLUDE_DIRS}
                    ${DIET_INCLUDE_DIR}
										${EMF_DATA_DIR}
                    ${UMS_DATA_DIR}
                    ${EMF4CPP_INCLUDE_DIR}
                    ${API_SOURCE_DIR}
                    ${CLIENT_PROXY_SOURCE_DIR}
                    ${VISHNU_EXCEPTION_INCLUDE_DIR}
                    ${CLI_SOURCE_DIR}
              )

add_library(cli-utils
 Options
 utils
 sessionUtils
 connectUtils
 configureOptionUtils
 localAccountUtils
 userUtils
 machineUtils
 listMachineUtils
 listHistoryCmdUtils
)
#declare all command lines provided

set(ALLCLI connect reconnect close
		    add_user update_user delete_user
				change_password reset_password
				 add_local_account update_local_account delete_local_account
				 save_configuration restore_configuration
				 add_machine update_machine delete_machine
				 configure_default_option configure_option
				 list_local_account list_machine list_history_cmd list_options list_users list_sessions)

#set the prefix of command
set (CMDPREFIX vishnu_)

# macro to generate all command lines

macro(genCli NAME )

	set(source_files ${NAME}
			)

set(CMDNAME ${CMDPREFIX}${NAME})

	#Configuration de l'executable
	add_executable(
	${CMDNAME}
	${source_files}
	)

	#Configuration de l'edition de liens
	target_link_libraries(
	${CMDNAME}
	${Boost_LIBRARIES}
        ${VISHNU_EXCEPTION_LIB}
        cli-utils
        ums-api-client
				crypt
        )

        install_targets(/bin/
         ${CMDNAME}
        )

endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})

genCli(${cli})

endforeach(cli)

install( TARGETS cli-utils DESTINATION lib )


