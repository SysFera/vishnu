#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;

const std::string configFileDir(TESTCORECONFIGPATH);
const std::string configFilePath=configFileDir+"/soci/";

static SOCIDatabase * myDatabase=NULL;

BOOST_AUTO_TEST_CASE( initialisation )
{
	//TODO : this should be in a Fixture class

	BOOST_TEST_MESSAGE("===== SOCIDatabase unit test (2): transactions =====");
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

BOOST_AUTO_TEST_CASE( starting_transactions )
{
	BOOST_REQUIRE(myDatabase != NULL);

	// starting transaction with disconnected database
	BOOST_TEST_MESSAGE("--- start transaction to disconnected database ---");
	BOOST_CHECK_THROW(myDatabase->startTransaction(),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	// starting transactions, maximum = 5, set in configuration file
	BOOST_TEST_MESSAGE("--- start transaction to connected database ---");
	BOOST_CHECK(myDatabase->startTransaction()==0);
	BOOST_TEST_MESSAGE("--- start transactions unit pool size limit ---");
	BOOST_CHECK(myDatabase->startTransaction()==1);
	BOOST_CHECK(myDatabase->startTransaction()==2);
	BOOST_CHECK(myDatabase->startTransaction()==3);
	BOOST_CHECK(myDatabase->startTransaction()==4);
	// try to get another transaction
	BOOST_CHECK_THROW(myDatabase->startTransaction(),VishnuException);
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( cancelling_transactions )
{
	BOOST_REQUIRE(myDatabase != NULL);

	// cancelling transaction with disconnected database
	BOOST_TEST_MESSAGE("--- cancel transaction from disconnected database ---");
	BOOST_CHECK_THROW(myDatabase->cancelTransaction(0),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->startTransaction()==0);
	BOOST_TEST_MESSAGE("--- cancel valid transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->cancelTransaction(0));
	// transaction id 5 is out of connection pool size
	BOOST_TEST_MESSAGE("--- cancel invalid transaction ---");
	BOOST_CHECK_THROW(myDatabase->cancelTransaction(5),VishnuException);
	// transaction 0 is already cancelled
	BOOST_TEST_MESSAGE("--- cancel non-started transaction ---");
	BOOST_CHECK_THROW(myDatabase->cancelTransaction(0),VishnuException);
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( ending_transactions )
{
	BOOST_REQUIRE(myDatabase != NULL);

	// ending transaction with disconnected database
	BOOST_TEST_MESSAGE("--- end transaction from disconnected database ---");
	BOOST_CHECK_THROW(myDatabase->endTransaction(0),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->startTransaction()==0);
	BOOST_TEST_MESSAGE("--- end valid transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->endTransaction(0));
	// transaction id 5 is out of connection pool size
	BOOST_TEST_MESSAGE("--- end invalid transaction ---");
	BOOST_CHECK_THROW(myDatabase->endTransaction(5),VishnuException);
	// transaction 0 is already cancelled
	BOOST_TEST_MESSAGE("--- end non-started transaction ---");
	BOOST_CHECK_THROW(myDatabase->endTransaction(0),VishnuException);
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( flushing_transactions )
{
	BOOST_REQUIRE(myDatabase != NULL);

	// ending transaction with disconnected database
	BOOST_TEST_MESSAGE("--- flush transaction from disconnected database ---");
	BOOST_CHECK_THROW(myDatabase->flush(0),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->startTransaction()==0);
	BOOST_TEST_MESSAGE("--- flush valid transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->flush(0));
	// transaction id 5 is out of connection pool size
	BOOST_TEST_MESSAGE("--- flush invalid transaction ---");
	BOOST_CHECK_THROW(myDatabase->flush(5),VishnuException);
	// a transaction can be flushed many times
	BOOST_CHECK_NO_THROW(myDatabase->flush(0));
	BOOST_CHECK_NO_THROW(myDatabase->endTransaction(0));
	// flush an inactive transaction start it
	BOOST_TEST_MESSAGE("--- flush inactive transaction ---");
	BOOST_CHECK_NO_THROW(myDatabase->flush(0));
	// disconnect from database
	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_SUITE_END()
