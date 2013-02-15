#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include "DIET_client.cpp"


BOOST_AUTO_TEST_SUITE( DIET_client_mock_unit_tests )

BOOST_AUTO_TEST_CASE( my_test_get_n_ums )
{
  BOOST_REQUIRE_EQUAL(get_module("sessionConnect"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("sessionReconnect"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("sessionClose"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("userCreate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("userUpdate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("userDelete"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("userPasswordChange"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("userPasswordReset"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("machineCreate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("machineUpdate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("machineDelete"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("localAccountCreate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("localAccountUpdate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("localAccountDelete"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("configurationSave"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("configurationRestore"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("optionValueSet"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("optionValueSetDefault"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("sessionList"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("localAccountList"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("machineList"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("commandList"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("optionValueList"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("userList"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("restore"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authSystemCreate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authSystemUpdate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authSystemDelete"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authSystemList"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authAccountCreate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authAccountUpdate"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authAccountDelete"), "UMS");
  BOOST_REQUIRE_EQUAL(get_module("authAccountList"), "UMS");
}

BOOST_AUTO_TEST_CASE( my_test_get_n_fms )
{
  BOOST_REQUIRE_EQUAL(get_module("DirCreate"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("DirList"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("DirRemove"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileChangeGroup"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileChangeMode"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileContent"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileCopy"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileCopyAsync"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileCreate"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileGetInfos"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileHead"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileMove"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileMoveAsync"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileRemove"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileTail"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileTransferStop"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("FileTransfersList"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("RemoteFileCopy"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("RemoteFileCopyAsync"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("RemoteFileMove"), "FMS");
  BOOST_REQUIRE_EQUAL(get_module("RemoteFileMoveAsync"), "FMS");
}

BOOST_AUTO_TEST_CASE( my_test_get_n_tms )
{
  BOOST_REQUIRE_EQUAL(get_module("jobSubmit"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobCancel"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobInfo"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("getListOfJobs"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("getJobsProgression"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("getListOfQueues"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobOutputGetResult"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobOutputGetCompletedJobs"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("getListOfJobs_all"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobSubmit_autom"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("addWork"), "TMS");
}

BOOST_AUTO_TEST_CASE( my_test_get_n_tms_at )
{
  BOOST_REQUIRE_EQUAL(get_module("jobSubmit@cluster"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobCancel@cluster"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobInfo@cluster"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("getListOfJobs@cluster"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("getJobsProgression@cluster"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("getListOfQueues@cluster"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobOutputGetResult@cluster"), "TMS");
  BOOST_REQUIRE_EQUAL(get_module("jobOutputGetCompletedJobs@cluster"), "TMS");
}

BOOST_AUTO_TEST_CASE( my_test_get_n_ims )
{
  BOOST_REQUIRE_EQUAL(get_module("int_exportCommands"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_getMetricCurentValue"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_getMetricHistory"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_getProcesses"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_setSystemInfo"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_setSystemThreshold"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_getSystemThreshold"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_defineUserIdentifier"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_defineJobIdentifier"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_defineTransferIdentifier"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_defineMachineIdentifier"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_loadShed"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_setUpdateFrequency"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_getUpdateFrequency"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_restart"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_stop"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_getSystemInfo"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_defineAuthIdentifier"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_defineWorkIdentifier"), "IMS");
}

BOOST_AUTO_TEST_CASE( my_test_get_n_ims_at )
{
  BOOST_REQUIRE_EQUAL(get_module("int_getMetricCurentValue@cluster"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_loadShed@cluster"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_restart@cluster"), "IMS");
  BOOST_REQUIRE_EQUAL(get_module("int_stop@cluster"), "IMS");
}

BOOST_AUTO_TEST_CASE( my_test_get_b_mod )
{
  BOOST_REQUIRE_EQUAL(get_module("bad"), "");
}

BOOST_AUTO_TEST_CASE( my_test_alloc_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
  BOOST_REQUIRE_EQUAL(prof->IN, 1);
  BOOST_REQUIRE_EQUAL(prof->INOUT, 2);
  BOOST_REQUIRE_EQUAL(prof->OUT, 3);
  BOOST_REQUIRE_EQUAL(std::string(prof->name), "alloc");
}

BOOST_AUTO_TEST_CASE( my_test_free_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
  BOOST_REQUIRE_EQUAL(prof->IN, 1);
  BOOST_REQUIRE_EQUAL(prof->INOUT, 2);
  BOOST_REQUIRE_EQUAL(prof->OUT, 3);
  BOOST_REQUIRE_EQUAL(std::string(prof->name), "alloc");

  BOOST_REQUIRE_EQUAL(diet_profile_free(prof), 0);
}



BOOST_AUTO_TEST_CASE( my_test_setStr_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
  BOOST_REQUIRE_NO_THROW(diet_string_set(prof, 0, "toto"));

  BOOST_REQUIRE_EQUAL(prof->params[0], "toto");
}

BOOST_AUTO_TEST_CASE( my_test_setStr_out_of_bound )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
  BOOST_REQUIRE_THROW(diet_string_set(prof, 10), SystemException);
}

BOOST_AUTO_TEST_CASE( my_test_getStr_n )
{
  std::string userId = "";
  diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
  diet_string_set(prof, 0, "toto");
  diet_string_get(prof, 0, userId);

  BOOST_REQUIRE_EQUAL(userId, "toto");
}

BOOST_AUTO_TEST_CASE( my_test_serial_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 0, 0, 1);
  diet_string_set(prof, 0, "param1");
  diet_string_set(prof, 1);
  std::string res = my_serialize(prof);

  BOOST_REQUIRE_EQUAL(res, "alloc$$$0$$$0$$$1$$$param1$$$$$$");
}

BOOST_AUTO_TEST_CASE( my_test_serial_b )
{
  BOOST_REQUIRE_THROW(my_serialize(NULL), SystemException);
}


BOOST_AUTO_TEST_CASE( my_test_deser_n )
{
  std::string profSer = "alloc$$$0$$$0$$$1$$$param1$$$$$$";
  boost::shared_ptr<diet_profile_t> prof = my_deserialize(profSer);
  BOOST_REQUIRE_EQUAL(prof->name, "alloc");
  BOOST_REQUIRE_EQUAL(prof->IN, 0);
  BOOST_REQUIRE_EQUAL(prof->INOUT, 0);
  BOOST_REQUIRE_EQUAL(prof->OUT, 1);
  BOOST_REQUIRE_EQUAL(prof->params[0], "param1");
}

BOOST_AUTO_TEST_CASE( my_test_deser_b_emp )
{
  std::string profSer;
  BOOST_REQUIRE_THROW(my_deserialize(profSer), SystemException);
}

BOOST_AUTO_TEST_CASE( my_test_init_b_nul )
{
  BOOST_REQUIRE_THROW(diet_initialize(NULL, 0, NULL), SystemException);
}

BOOST_AUTO_TEST_CASE( my_test_init_b_invalid )
{
  BOOST_REQUIRE_THROW(diet_initialize("/tmp/toto", 0, NULL), UserException);
}

BOOST_AUTO_TEST_SUITE_END()
