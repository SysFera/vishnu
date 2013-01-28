

include(UnitTest)
if(COMPILE_CLIENT_CLI AND COMPILE_SERVERS)
unit_test(utilVishnuUnitTests vishnu-core)
unit_test(utilServerUnitTests vishnu-core vishnu-core-server)

endif()

