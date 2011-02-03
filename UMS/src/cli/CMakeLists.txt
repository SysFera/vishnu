
link_directories(${Boost_LIBRARY_DIRS}
                  ${EMF4CPP_LIB}
                  ${EMF4CPP_CPP_LIB}
                  ${EMF4CPP_LIB} 
                  ${EMF4CPP_CPP_LIB}
                )
include_directories(${Boost_INCLUDE_DIRS}
                    ${DIET_DIR}/include
                    ${DIET_INCLUDE_DIR}
                    ${DIET_DIR}/include 
	            ${EMF_DATA_DIR}
                    ${UMS_DATA_DIR}
                    ${EMF4CPP_CPP_DIR}/include/emf4cpp
                    ${EMF4CPP_INCLUDE_DIR}
                    ${CLIENT_PROXY_SOURCE_DIR}                   
              )

#declare all command lines provided

set(ALLCLI connect reconnect close
		    addVishnuUser updateUser deleteUser)


# macro to generate all command lines

macro(genCli NAME )
	set(source_files ${NAME}
			Options )
#api)

	#Configuration de l'executable
	add_executable(
	${NAME}
	${source_files}
	)

	#Configuration de l'edition de liens
	target_link_libraries(
	${NAME}
	${Boost_LIBRARIES}
        ${EMF4CPP_UMS_DATA_LIB}
        umsclient
        )
endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})

genCli(${cli})

endforeach(cli)




