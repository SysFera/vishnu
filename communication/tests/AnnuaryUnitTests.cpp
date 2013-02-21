#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include <boost/make_shared.hpp>
#include "Annuary.hpp"

std::vector<boost::shared_ptr<Server> > mservers;
std::string name = "pierre";
std::string uri = "tcp://*:5555";
std::vector<std::string> services;
std::vector<std::string> cfgInfo;


class AnnuaryFixture {
public:
  AnnuaryFixture(){
    name = "pierre";
    uri = "tcp://*:5555";
    services.clear();
    services.push_back("loup");
    services.push_back("belette");

    cfgInfo.push_back("tcp://127.0.0.1 cluster1");
    cfgInfo.push_back("tcp://192.168.0.2 cluster2");

    mservers.clear();
    mservers.push_back(boost::make_shared<Server>(name, services, uri));
  }

  ~AnnuaryFixture(){
    mservers.clear();
    cfgInfo.clear();
    services.clear();
  }
};




BOOST_FIXTURE_TEST_SUITE( annuary_unit_tests, AnnuaryFixture )


BOOST_AUTO_TEST_CASE( test_default_constructor_n )
{
  Annuary ann;
  BOOST_REQUIRE_EQUAL(ann.get().size(), 0);
}


BOOST_AUTO_TEST_CASE( test_add_n )
{
// Test the annuary class
  Annuary ann(mservers);
  std::vector<std::string> servicesTmp;
  servicesTmp.push_back("toto");
  ann.add("titi", "tutu", servicesTmp);
  BOOST_REQUIRE_EQUAL(ann.get("toto").size(), 1);
}


BOOST_AUTO_TEST_CASE( test_remove_n )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name, uri);
  BOOST_REQUIRE(ann.get().empty());
}

BOOST_AUTO_TEST_CASE( test_get_n )
{
// Test the annuary class
  Annuary ann(mservers);
  BOOST_REQUIRE(!ann.get("loup").empty());
  BOOST_REQUIRE(!ann.get("belette").empty());
}

BOOST_AUTO_TEST_CASE( test_get_bad )
{
// Test the annuary class
  Annuary ann(mservers);
  BOOST_REQUIRE(ann.get("bad").empty());
}

BOOST_AUTO_TEST_CASE( test_remove_bad_name )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name+"1", uri);
  BOOST_REQUIRE(ann.get("loup").size() == 1);
}

BOOST_AUTO_TEST_CASE( test_remove_bad_uri )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name, uri+"1");
  BOOST_REQUIRE(ann.get("loup").size() == 1);
}

BOOST_AUTO_TEST_CASE( test_add_bad_twice )
{
// Test the annuary class
  Annuary ann(mservers);
  std::vector<std::string> servicesTmp;
  servicesTmp.push_back("toto");
  ann.add("titi", "tutu", servicesTmp);
  ann.add("titi", "tutu", servicesTmp);
  BOOST_REQUIRE(ann.get().size() == 2);
}

BOOST_AUTO_TEST_CASE( test_remove_bad_empty )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name, uri);
  ann.remove(name, uri);
  BOOST_REQUIRE(ann.get().empty());
}

BOOST_AUTO_TEST_CASE( test_setInitConfig_IMS_n )
{
  Annuary ann;
  ann.setInitConfig("IMS", cfgInfo);

  BOOST_REQUIRE_EQUAL(ann.get("int_exportCommands").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_getMetricHistory").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_getProcesses").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_setSystemInfo").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_setSystemThreshold").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_getSystemThreshold").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_defineUserIdentifier").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_defineJobIdentifier").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_defineTransferIdentifier").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_defineMachineIdentifier").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_setUpdateFrequency").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_getUpdateFrequency").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_getSystemInfo").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_defineAuthIdentifier").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("int_defineWorkIdentifier").size(), 2);

  BOOST_REQUIRE(!ann.get("int_getMetricCurentValue@cluster1").empty());
  BOOST_REQUIRE(!ann.get("int_loadShed@cluster1").empty());
  BOOST_REQUIRE(!ann.get("int_restart@cluster1").empty());
  BOOST_REQUIRE(!ann.get("int_stop@cluster1").empty());

  BOOST_REQUIRE(!ann.get("int_getMetricCurentValue@cluster2").empty());
  BOOST_REQUIRE(!ann.get("int_loadShed@cluster2").empty());
  BOOST_REQUIRE(!ann.get("int_restart@cluster2").empty());
  BOOST_REQUIRE(!ann.get("int_stop@cluster2").empty());
}

BOOST_AUTO_TEST_CASE( test_setInitConfig_TMS_n )
{
  Annuary ann;
  ann.setInitConfig("TMS", cfgInfo);

  BOOST_REQUIRE(!ann.get("jobSubmit@cluster1").empty());
  BOOST_REQUIRE(!ann.get("jobCancel@cluster1").empty());
  BOOST_REQUIRE(!ann.get("jobInfo@cluster1").empty());
  BOOST_REQUIRE(!ann.get("getListOfJobs@cluster1").empty());
  BOOST_REQUIRE(!ann.get("getJobsProgression@cluster1").empty());
  BOOST_REQUIRE(!ann.get("getListOfQueues@cluster1").empty());
  BOOST_REQUIRE(!ann.get("jobOutputGetResult@cluster1").empty());
  BOOST_REQUIRE(!ann.get("jobOutputGetCompletedJobs@cluster1").empty());

  BOOST_REQUIRE(!ann.get("jobSubmit@cluster2").empty());
  BOOST_REQUIRE(!ann.get("jobCancel@cluster2").empty());
  BOOST_REQUIRE(!ann.get("jobInfo@cluster2").empty());
  BOOST_REQUIRE(!ann.get("getListOfJobs@cluster2").empty());
  BOOST_REQUIRE(!ann.get("getJobsProgression@cluster2").empty());
  BOOST_REQUIRE(!ann.get("getListOfQueues@cluster2").empty());
  BOOST_REQUIRE(!ann.get("jobOutputGetResult@cluster2").empty());
  BOOST_REQUIRE(!ann.get("jobOutputGetCompletedJobs@cluster2").empty());
}

BOOST_AUTO_TEST_CASE( test_setInitConfig_FMS_n )
{
  Annuary ann;
  ann.setInitConfig("FMS", cfgInfo);

  BOOST_REQUIRE_EQUAL(ann.get("DirCreate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("DirList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("DirRemove").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileChangeGroup").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileChangeMode").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileContent").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileCopy").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileCopyAsync").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileCreate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileGetInfos").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileHead").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileMove").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileMoveAsync").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileRemove").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileTail").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileTransferStop").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("FileTransfersList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("RemoteFileCopy").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("RemoteFileCopyAsync").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("RemoteFileMove").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("RemoteFileMoveAsync").size(), 2);
}

BOOST_AUTO_TEST_CASE( test_setInitConfig_UMS_n )
{
  Annuary ann;
  ann.setInitConfig("UMS", cfgInfo);

  BOOST_REQUIRE_EQUAL(ann.get("sessionConnect").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("sessionReconnect").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("sessionClose").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("userCreate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("userUpdate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("userDelete").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("userPasswordChange").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("userPasswordReset").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("machineCreate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("machineUpdate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("machineDelete").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("localAccountCreate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("localAccountUpdate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("localAccountDelete").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("configurationSave").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("configurationRestore").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("optionValueSet").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("optionValueSetDefault").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("sessionList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("localAccountList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("machineList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("commandList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("optionValueList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("userList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("restore").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authSystemCreate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authSystemUpdate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authSystemDelete").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authSystemList").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authAccountCreate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authAccountUpdate").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authAccountDelete").size(), 2);
  BOOST_REQUIRE_EQUAL(ann.get("authAccountList").size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
