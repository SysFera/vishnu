include_directories(
    ${VISHNU_SOURCE_DIR}/TMS/src/posix_parser
    ${VISHNU_SOURCE_DIR}/TMS/src/server
    
)

include(UnitTest)
if(COMPILE_CLIENT_CLI AND COMPILE_SERVERS)
unit_test(POSIXParserUnitTests ${POSIX_TEST_LIB})
unit_test(EnvUnitTests vishnu-tms-server)
unit_test(ScriptGenConvertorUnitTests vishnu-tms-server)
#unit_test(JobServerUnitTests vishnu-tms-server)
#unit_test(ParserUnitTests vishnu-tms-server)

endif()
