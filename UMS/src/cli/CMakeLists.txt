
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
)
#declare all command lines provided

set(ALLCLI connect reconnect close
		    addVishnuUser updateUser deleteUser
				changePassword resetPassword
				 addLocalAccount updateLocalAccount deleteLocalAccount
				 saveConfiguration restoreConfiguration
				 addMachine updateMachine deleteMachine
				 configureDefaultOption configureOption
				 listLocalAccount listMachine listHistoryCmd listOptions listUsers listSessions)


# macro to generate all command lines

macro(genCli NAME )
	set(source_files ${NAME}
			)

	#Configuration de l'executable
	add_executable(
	${NAME}
	${source_files}
	)

	#Configuration de l'edition de liens
	target_link_libraries(
	${NAME}
	${Boost_LIBRARIES}
        ${VISHNU_EXCEPTION_LIB}
        cli-utils
        ums-api-client
        )

        install_targets(/bin/
         ${NAME}
        )

endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})

genCli(${cli})

endforeach(cli)




