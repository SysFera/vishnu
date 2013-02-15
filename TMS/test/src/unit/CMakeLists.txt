include_directories(
    ${VISHNU_SOURCE_DIR}/TMS/src/posix_parser
    ${VISHNU_SOURCE_DIR}/TMS/src/server
)

include(UnitTest)
if(COMPILE_CLIENT_CLI AND COMPILE_SERVERS)
unit_test(POSIXParserUnitTests ${POSIX_TEST_LIB})
unit_test(EnvUnitTests vishnu-tms-server)
endif()
