#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"
#include "DbFactory.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;

const std::string configFileDir(TESTCORECONFIGPATH);
const std::string configFilePath=configFileDir+"/soci/";



/*
 * DbFactory unit tests
 */


BOOST_AUTO_TEST_CASE( database_factory_get_before_create )
{
	BOOST_TEST_MESSAGE("===== DbFactory unit test =====");
	BOOST_TEST_MESSAGE("--- get database instance before create it--- ");
	DbFactory factory;
	BOOST_CHECK_THROW(factory.getDatabaseInstance(),VishnuException);
}


BOOST_AUTO_TEST_CASE( database_factory_create )
{
	BOOST_TEST_MESSAGE("--- create database instance ---");
	DbFactory factory;
	ExecConfiguration exConfig;
	DbConfiguration myConfig(exConfig);
	BOOST_CHECK(factory.createDatabaseInstance(myConfig)!=NULL);
	BOOST_CHECK_THROW(factory.createDatabaseInstance(myConfig),VishnuException);
}

BOOST_AUTO_TEST_CASE( database_factory_get_after_create )
{
	BOOST_TEST_MESSAGE("--- get database instance ---");
	DbFactory factory;
	BOOST_CHECK(factory.getDatabaseInstance()!=NULL);
	BOOST_CHECK(factory.getDatabaseInstance()!=NULL);
	BOOST_CHECK(factory.getDatabaseInstance()==factory.getDatabaseInstance());
}


/*
 * SOCIDatabase unit tests - connections
 */
BOOST_AUTO_TEST_CASE( connect_and_disconnect )
{
	BOOST_TEST_MESSAGE("===== SOCIDatabase unit tests (1) : connections =====");

	BOOST_TEST_MESSAGE("--- valid connection to database ---");
	ExecConfiguration exConfig;
	std::string configFile=configFilePath+"soci-test_config.cfg";
	exConfig.initFromFile(configFile);
	DbConfiguration myConfig(exConfig);
	myConfig.check();
	SOCIDatabase myDatabase(myConfig);
	BOOST_CHECK(myDatabase.connect()==0);
	BOOST_CHECK(myDatabase.disconnect()==0);

	BOOST_TEST_MESSAGE("--- disconnection from connected database ---");
	BOOST_CHECK_THROW(myDatabase.disconnect(),VishnuException);
}

BOOST_AUTO_TEST_CASE( connect_with_bad_database )
{
	BOOST_TEST_MESSAGE("--- connection to unexistant database ---");
	ExecConfiguration exBadConfig;
	std::string badConfigFile=configFilePath+"soci-test_bad_DB.cfg";
	exBadConfig.initFromFile(badConfigFile);
	DbConfiguration badConfig(exBadConfig);
	badConfig.check();
	SOCIDatabase badDB(badConfig);
	BOOST_CHECK_THROW(badDB.connect(),VishnuException);
	BOOST_TEST_MESSAGE("--- disconnect from non-connected database ---");
	BOOST_CHECK_THROW(badDB.disconnect(),VishnuException);

}

BOOST_AUTO_TEST_CASE( connect_with_bad_user )
{
	BOOST_TEST_MESSAGE("--- connection with bad user login ---");
	ExecConfiguration exBadConfig;
	std::string badConfigFile=configFilePath+"soci-test_bad_user.cfg";
	exBadConfig.initFromFile(badConfigFile);
	DbConfiguration badUserConfig(exBadConfig);
	badUserConfig.check();
	SOCIDatabase badUser(badUserConfig);
	BOOST_CHECK_THROW(badUser.connect(),VishnuException);
	BOOST_CHECK_THROW(badUser.disconnect(),VishnuException);
}

BOOST_AUTO_TEST_CASE( connect_with_bad_password )
{
	BOOST_TEST_MESSAGE("--- connection with bad user password ---");
	ExecConfiguration exBadConfig;
	std::string badConfigFile=configFilePath+"soci-test_bad_password.cfg";
	exBadConfig.initFromFile(badConfigFile);
	DbConfiguration badPasswordConfig(exBadConfig);
	badPasswordConfig.check();
	SOCIDatabase badPassword(badPasswordConfig);
	BOOST_CHECK_THROW(badPassword.connect(),VishnuException);
	BOOST_CHECK_THROW(badPassword.disconnect(),VishnuException);
}

BOOST_AUTO_TEST_CASE( connect_with_bad_hosts )
{
	BOOST_TEST_MESSAGE("--- connection with invalid host ---");
	ExecConfiguration exBadConfig;
	std::string badConfigFile=configFilePath+"soci-test_bad_host.cfg";
	exBadConfig.initFromFile(badConfigFile);
	DbConfiguration badHostConfig(exBadConfig);
	badHostConfig.check();
	SOCIDatabase badHost(badHostConfig);
	BOOST_CHECK_THROW(badHost.connect(),VishnuException);
	BOOST_CHECK_THROW(badHost.disconnect(),VishnuException);

}

BOOST_AUTO_TEST_SUITE_END()
