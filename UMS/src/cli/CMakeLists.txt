
link_directories(${Boost_LIBRARY_DIRS}
                  ${EMF4CPP_LIB}
                  ${EMF4CPP_CPP_LIB}
                )
include_directories(${Boost_INCLUDE_DIRS}
                    ${DIET_DIR}/include
                    ${DIET_INCLUDE_DIR}
	            ${EMF_DATA_DIR}
                    ${UMS_DATA_DIR}
                    ${EMF4CPP_INCLUDE_DIR}
                    ${API_SOURCE_DIR} 
                    ${CLIENT_PROXY_SOURCE_DIR} 
                    ${VISHNU_EXCEPTION_INCLUDE_DIR} 
              )

#declare all command lines provided

set(ALLCLI connect reconnect close
		    addVishnuUser updateUser deleteUser)


# macro to generate all command lines

macro(genCli NAME )
	set(source_files ${NAME}
			Options)

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




