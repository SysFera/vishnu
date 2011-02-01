cmake_minimum_required(VERSION 2.6)

#Configuration du projet
project(ProgOptionTest)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE})

#Inclusion de Boost
include(FindBoost)
find_package(
	Boost
	1.40.0
	REQUIRED program_options
)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

#include_directories(${boost_include_dir})
#link_directories(${boost_lib_dir})

macro(genCli NAME )
	set(source_files ${NAME} Options)

	#Configuration de l'executable
	add_executable(
	${NAME}
	${source_files}
	)

	#Configuration de l'edition de liens
	target_link_libraries(
	${NAME}
	${Boost_LIBRARIES}

	)
endmacro(genCli)

genCli(connect)
genCli(reconnect)
genCli(close)

