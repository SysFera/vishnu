# wrapper allowing us to launch tests and redirect their output

# don't use $ENV{XXX} as in documentation ==> does not work
set( ENV{BOOST_TEST_CATCH_SYSTEM_ERRORS} "no" )
set(ENV{VISHNU_CONFIG_FILE} "${VISHNU_CONFIG}")
set(ENV{OMNIORB4_DIR} "/usr/local")
set(ENV{OMNIORB4_INCLUDE_DIR} "$ENV{OMNIORB4_DIR}/include")
set(ENV{OMNIORB_CONFIG} "$ENV{OMNIORB4_DIR}/etc/omniORB4.cfg")
set(ENV{OMNINAMES_LOGDIR} "/tmp")
set(ENV{PATH} "$ENV{OMNIORB4_DIR}/bin:$ENV{PATH}")

execute_process( COMMAND export )

execute_process( COMMAND ${BIN_PATH}/${TEST_PROG} 
  --result_code=no
  --report_level=no
  --log_format=xml
  --log_level=test_suite
  --log_sink=${REPORT_PATH}/${TEST_PROG}.xml
  OUTPUT_QUIET
  ERROR_QUIET
)
