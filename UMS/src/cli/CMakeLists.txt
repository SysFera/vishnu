cmake_minimum_required(VERSION 2.6)

#Configuration du projet
project(ProgOptionTest)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE})

#Inclusion de Boost
include(FindBoost)
find_package(
	Boost
	1.42.0
	REQUIRED program_options
)
link_directories(${Boost_LIBRARY_DIRS} 
#             ${EMF4CPP_LIB}
#                 ${EMF4CPP_CPP_LIB}
#                 ${EMF4CPP_LIB} 
#                 ${EMF4CPP_CPP_LIB}
		)
include_directories(${Boost_INCLUDE_DIRS}
#                   ${DIET_DIR}/include
#                    ${DIET_INCLUDE_DIR}
#                    ${DIET_DIR}/include 
#                  ${UMS_CLIENT_SOURCE_DIR}/utils/emfdata/
#                   ${UMS_CLIENT_SOURCE_DIR}/utils/emfdata/UMS_Data
#                   ${UMS_CLIENT_SOURCE_DIR}/client
#                   /opt/emf4cpp-0.0.2/include/emf4cpp
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
#   /home/traore/ProjectDev/UMS/UMS_client_test/build/utils/emfdata/libemf4cpp-UMS_Data.so
#        umsClientTest
	)
endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})

genCli(${cli})

endforeach(cli)




