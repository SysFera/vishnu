#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;

const std::string configFilePath(TESTCORECONFIGPATH);
static SOCIDatabase * myDatabase=NULL;

BOOST_AUTO_TEST_CASE( initialisation )
{
	//TODO : this should be in a Fixture class

	BOOST_TEST_MESSAGE("initialisation, connection to database");
	ExecConfiguration exConfig;
	std::string configFile=configFilePath+"soci-test_config.cfg";
	try{
		exConfig.initFromFile(configFile);
		DbConfiguration myConfig(exConfig);
		myConfig.check();
		myDatabase = new SOCIDatabase(myConfig);
	} catch( exception const & e)
	{
		BOOST_TEST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}
	BOOST_REQUIRE(myDatabase!=NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->disconnect()==0);
	BOOST_TEST_MESSAGE("Normal connection to Database -- ok");

	BOOST_CHECK_THROW(myDatabase->disconnect(),VishnuException);
}

BOOST_AUTO_TEST_CASE( getting_single_session )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	// processing with disconnected database
	BOOST_CHECK_THROW(myDatabase->getSingleSession(),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session;
	SOCISession session2;
	SOCISession session3;
	SOCISession nullSession;
	BOOST_CHECK_NO_THROW(session = myDatabase->getSingleSession());
	// get Single session with requested position
	BOOST_CHECK_NO_THROW(session2 = myDatabase->getSingleSession(2));
	BOOST_CHECK_THROW(session3 = myDatabase->getSingleSession(147),VishnuException);
	BOOST_CHECK(myDatabase->releaseSingleSession(session)==0);
	// single session with requested position does not need to be released
	BOOST_CHECK_THROW(myDatabase->releaseSingleSession(session2),VishnuException);
	BOOST_CHECK_THROW(myDatabase->releaseSingleSession(nullSession), VishnuException);


	// maximum : 5 sessions, set in configuration file
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_THROW(myDatabase->getSingleSession(),VishnuException);
	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( session_transaction_test )
{
	BOOST_REQUIRE(myDatabase != NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	try{
		SOCISession session;
		BOOST_CHECK_NO_THROW(session = myDatabase->getSingleSession());

		BOOST_CHECK_NO_THROW(session.rollback());
		BOOST_CHECK_NO_THROW(session.commit());
		BOOST_CHECK_NO_THROW(session.begin());
		BOOST_CHECK_NO_THROW(session.commit());
		BOOST_CHECK_NO_THROW(session.rollback());


		BOOST_CHECK_NO_THROW(myDatabase->releaseSingleSession(session));
	} catch (exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( clean_test )
{
	BOOST_REQUIRE(myDatabase != NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->process("drop table if exists paco")==0);
	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( finalize_test )
{
	// TODO this should be in a Fixture class

	try {
		if(myDatabase != NULL){
			delete myDatabase;
		}
		BOOST_CHECK(true);
	} catch (exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_REQUIRE(false);
	}


}

BOOST_AUTO_TEST_SUITE_END()
