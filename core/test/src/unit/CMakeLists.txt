include_directories(
   ${TMS_EMF_DATA_DIR}
   ${UTILVISHNU_SOURCE_DIR}
   ${EMF_DATA_DIR}
   ${EMF4CPP_INCLUDE_DIR}
   ${VISHNU_EXCEPTION_INCLUDE_DIR}
)



include(UnitTest)
if(COMPILE_CLIENT_CLI AND COMPILE_SERVERS)
unit_test(utilVishnuUnitTests vishnu-core vishnu-tms-client)
unit_test(utilServerUnitTests vishnu-core-server vishnu-core vishnu-tms-client)
unit_test(utilClientUnitTests vishnu-core vishnu-tms-client)

endif()

