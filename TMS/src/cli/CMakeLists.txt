
link_directories(${Boost_LIBRARY_DIRS}
                  ${TMS_CLI_SOURCE_DIR}
                  ${CLI_SOURCE_DIR}
		  ${LIBRARY_OUTPUT_PATH}
                )

include_directories(${Boost_INCLUDE_DIRS}
                    ${DIET_INCLUDE_DIR}
                    ${TMS_EMF_DATA_DIR}
                    ${TMS_DATA_DIR}
                    ${EMF4CPP_INCLUDE_DIR}
                    ${TMS_API_SOURCE_DIR}
                    ${TMS_CLIENT_PROXY_SOURCE_DIR}
                    ${VISHNU_EXCEPTION_INCLUDE_DIR}
                    ${TMS_CLI_SOURCE_DIR}
                    ${UTILVISHNU_SOURCE_DIR}
		    ${API_SOURCE_DIR}
		    ${CLIENT_PROXY_SOURCE_DIR}
		    ${EMF_DATA_DIR}
		    ${UMS_DATA_DIR}
                  )

add_library(tms-utils-cli
	    displayer
 )
#declare all command lines provided

set(ALLCLI 
  cancel_job get_job_info get_job_progress
  list_jobs set_machine_refresh_period
  get_machine_refresh_period
  get_all_job_output get_job_output
  list_queues submit_job
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
                        tms-utils-cli
                        utils-vishnu
                        vishnu-tms-api
                        vishnu-ums-api
                        crypt
                        -pthread
                        )
  install_targets(/bin/
                  ${CMDNAME}
                )

endmacro(genCli)

#Loop over all command lines to generate them

foreach(cli ${ALLCLI})

genCli(${cli})

endforeach(cli)

install( TARGETS tms-utils-cli DESTINATION lib )

