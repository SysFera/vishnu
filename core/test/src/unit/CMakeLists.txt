include_directories(
   ${TMS_EMF_DATA_DIR}
   ${UTILVISHNU_SOURCE_DIR}
   ${EMF_DATA_DIR}
   ${EMF4CPP_INCLUDE_DIR}
   ${VISHNU_EXCEPTION_INCLUDE_DIR}
)



include(UnitTest)
if(COMPILE_CLIENT_CLI AND COMPILE_SERVERS)
unit_test(utilVishnuUnitTests vishnu-core)
unit_test(tmsUtilsUnitTests vishnu-core)
unit_test(utilServerUnitTests vishnu-core-server vishnu-core)
unit_test(utilClientUnitTests vishnu-core)
unit_test(ExecConfigurationUnitTests vishnu-core-server vishnu-core)
endif()

