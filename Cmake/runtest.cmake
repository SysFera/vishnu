# wrapper allowing us to launch tests and redirect their output

# don't use $ENV{XXX} as in documentation ==> does not work
set( ENV{BOOST_TEST_CATCH_SYSTEM_ERRORS} "no" )
execute_process( COMMAND ${BIN_PATH}/${TEST_PROG}
  --report_format=xml
  --report_level=detailed
  #OUTPUT_FILE "${REPORT_PATH}/${TEST_PROG}.xml"
  ERROR_FILE "${REPORT_PATH}/${TEST_PROG}.xml"
)
