#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include <boost/make_shared.hpp>
#include "Annuary.hpp"
#include "UMSServices.hpp"
#include "FMSServices.hpp"

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
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_FMS; nb++) {
    BOOST_MESSAGE("Checking " << SERVICES_FMS[nb]);
    BOOST_REQUIRE_EQUAL(ann.get(SERVICES_FMS[nb]).size(), 2);
  }
}

BOOST_AUTO_TEST_CASE( test_setInitConfig_UMS_n )
{
  Annuary ann;
  ann.setInitConfig("UMS", cfgInfo);
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_UMS; nb++) {
    BOOST_MESSAGE("Checking " << SERVICES_UMS[nb]);
    BOOST_REQUIRE_EQUAL(ann.get(SERVICES_UMS[nb]).size(), 2);
  }
}

BOOST_AUTO_TEST_SUITE_END()
