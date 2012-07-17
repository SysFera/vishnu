#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;

const std::string configFilePath(TESTCORECONFIGPATH);

BOOST_AUTO_TEST_CASE( connect_and_disconnect )
{


	BOOST_TEST_MESSAGE("Testing normal connection to Database");
	ExecConfiguration exConfig;
	std::string configFile=configFilePath+"soci-test_config.cfg";
	exConfig.initFromFile(configFile);
	DbConfiguration myConfig(exConfig);
	myConfig.check();
	SOCIDatabase myDatabase(myConfig);
	BOOST_CHECK(myDatabase.connect()==0);
	BOOST_CHECK(myDatabase.disconnect()==0);
	BOOST_TEST_MESSAGE("Normal connection to Database -- ok");

	BOOST_CHECK_THROW(myDatabase.disconnect(),VishnuException);
}

BOOST_AUTO_TEST_CASE( connect_with_bad_database )
{
	ExecConfiguration exBadConfig;
	std::string badConfigFile=configFilePath+"soci-test_bad_DB.cfg";
	exBadConfig.initFromFile(badConfigFile);
	DbConfiguration badConfig(exBadConfig);
	badConfig.check();
	SOCIDatabase badDB(badConfig);
	BOOST_CHECK_THROW(badDB.connect(),VishnuException);
	BOOST_CHECK_THROW(badDB.disconnect(),VishnuException);

}

BOOST_AUTO_TEST_CASE( connect_with_bad_user )
{
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
	ExecConfiguration exBadConfig;
	std::string badConfigFile=configFilePath+"soci-test_bad_host.cfg";
	exBadConfig.initFromFile(badConfigFile);
	DbConfiguration badHostConfig(exBadConfig);
	badHostConfig.check();
	SOCIDatabase badHost(badHostConfig);
	BOOST_CHECK_THROW(badHost.connect(),VishnuException);
	BOOST_CHECK_THROW(badHost.disconnect(),VishnuException);

} // BOOST_AUTO_TEST_CASE( connect_and_disconnect )

BOOST_AUTO_TEST_SUITE_END()
