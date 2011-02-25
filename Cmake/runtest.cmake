# wrapper allowing us to launch tests and redirect their output

# don't use $ENV{XXX} as in documentation ==> does not work
set( ENV{BOOST_TEST_CATCH_SYSTEM_ERRORS} "no" )
set (ENV{VISHNU_CONFIG_FILE} "${VISHNU_CONFIG}")
set (ENV{OMNIORB4_DIR} "/usr/local")
set (ENV{OMNIORB4_INCLUDE_DIR} "$ENV{OMNIORB4_DIR}/include")
set (ENV{OMNIORB_CONFIG} "$ENV{OMNIORB4_DIR}/etc/omniORB4.cfg")
set (ENV{OMNINAMES_LOGDIR} "/tmp")
set (ENV{PATH} "$ENV{OMNIORB4_DIR}/bin:$PATH")

execute_process( COMMAND export )

execute_process( COMMAND ${BIN_PATH}/${TEST_PROG}
  --report_format=xml
  --report_level=detailed
  #OUTPUT_FILE "${REPORT_PATH}/${TEST_PROG}.xml"
  ERROR_FILE "${REPORT_PATH}/${TEST_PROG}.xml"
)
