#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;


BOOST_AUTO_TEST_CASE( connect_and_disconnect )
{

	cout<<"#### TEST 0 ####"<<endl;
	ExecConfiguration exBadConfig;
	exBadConfig.initFromFile("/home/jbghoul/conf/bad_config/soci-test_bad_DB.cfg");
	DbConfiguration badConfig(exBadConfig);
	badConfig.check();
	SOCIDatabase badDB(badConfig);
	BOOST_CHECK_THROW(badDB.connect(),VishnuException);
	BOOST_CHECK_THROW(badDB.disconnect(),VishnuException);


	exBadConfig.initFromFile("/home/jbghoul/conf/bad_config/soci-test_bad_user.cfg");
	DbConfiguration badUserConfig(exBadConfig);
	badUserConfig.check();
	SOCIDatabase badUser(badUserConfig);
	BOOST_CHECK_THROW(badUser.connect(),VishnuException);
	BOOST_CHECK_THROW(badUser.disconnect(),VishnuException);


	exBadConfig.initFromFile("/home/jbghoul/conf/bad_config/soci-test_bad_password.cfg");
	DbConfiguration badPasswordConfig(exBadConfig);
	badPasswordConfig.check();
	SOCIDatabase badPassword(badPasswordConfig);
	BOOST_CHECK_THROW(badPassword.connect(),VishnuException);
	BOOST_CHECK_THROW(badPassword.disconnect(),VishnuException);


	exBadConfig.initFromFile("/home/jbghoul/conf/bad_config/soci-test_bad_host.cfg");
	DbConfiguration badHostConfig(exBadConfig);
	badHostConfig.check();
	SOCIDatabase badHost(badHostConfig);
	BOOST_CHECK_THROW(badHost.connect(),VishnuException);
	BOOST_CHECK_THROW(badHost.disconnect(),VishnuException);


	ExecConfiguration exConfig;
	exConfig.initFromFile("/home/jbghoul/conf/soci-test_config.cfg");
	DbConfiguration myConfig(exConfig);
	myConfig.check();
	SOCIDatabase myDatabase(myConfig);
	BOOST_CHECK(myDatabase.connect()==0);
	BOOST_CHECK(myDatabase.disconnect()==0);


} // BOOST_AUTO_TEST_CASE( my_test )

BOOST_AUTO_TEST_SUITE_END()
