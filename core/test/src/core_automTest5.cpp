#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;

const std::string configFileDir(TESTCORECONFIGPATH);
const std::string configFilePath=configFileDir+"/soci/";

static SOCIDatabase * myDatabase=NULL;

/*
 * SOCIStatement unit tests and functional tests
 */
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

BOOST_AUTO_TEST_CASE( getting_single_statement )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();
	// getting a single statement
	BOOST_CHECK_NO_THROW(statement=session.getStatement());

	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( prepare_statement_test)
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();

	BOOST_CHECK_NO_THROW(statement.prepare(""));
	BOOST_CHECK_NO_THROW(statement.prepare("drop table if exists paco;"));

	// multiple commands in prepare statement is not allowed
	BOOST_CHECK_THROW(statement.prepare("create table jambon(cru int);"),VishnuException);

	// bad synthax
	SOCIStatement statement2=session.getStatement();
	BOOST_CHECK_THROW(statement2.prepare("bad syntax"),VishnuException);
	// SQL error -- sometablewhichdoesnotexist does not exists
	SOCIStatement statement3=session.getStatement();
	BOOST_CHECK_THROW(statement3.prepare("select something from sometablewhichdoesnotexist")
			,VishnuException);


	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( alloc_statement_test)
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();

	statement.alloc();
	statement.alloc();

	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( clean_up_statement )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();

	BOOST_CHECK_NO_THROW(statement.clean_up());
	// after clean up, statement has no backend
	BOOST_CHECK_NO_THROW(statement.clean_up());
	BOOST_CHECK_THROW(statement.prepare("drop table if exists paco"),VishnuException);
	BOOST_CHECK_THROW(statement.execute(false),VishnuException);
	BOOST_CHECK_THROW(statement.alloc(),VishnuException);
	BOOST_CHECK_NO_THROW(statement.define_and_bind());


	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( executing_statement_test )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();


	BOOST_CHECK_NO_THROW(statement.prepare("drop table if exists paco"));
	BOOST_CHECK(statement.execute(false)==false);
	// second call - SQL request appends to previous -- synthax error
	BOOST_CHECK_THROW(statement.prepare("drop table if exists paco"),VishnuException);
	BOOST_CHECK_THROW(statement.execute(false),VishnuException);


	SOCIStatement statement2=session.getStatement();
	BOOST_CHECK_NO_THROW(statement2.prepare("create table paco(id int, nom varchar(255));"));
	BOOST_CHECK(statement2.execute(false)==false);

	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( repeating_statement_with_input )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();

	BOOST_CHECK_NO_THROW(statement.prepare("insert into paco(id,nom) values (:id,:name)"));
	int i =0;
	string name="";
	BOOST_CHECK_NO_THROW(statement.exchange_use(i));
	BOOST_CHECK_NO_THROW(statement.exchange_use(name));
	BOOST_CHECK_NO_THROW(statement.alloc());
	BOOST_CHECK_NO_THROW(statement.define_and_bind());

	for(i=0;i<10;++i)
	{
		name+="*";
		BOOST_CHECK_NO_THROW(statement.execute(true));
	}
	name.clear();
	// with no data exchange TODO : understand what does it mean
	for(i=10;i<20;++i)
	{
		name+="+";
		BOOST_CHECK_NO_THROW(statement.execute(false));
	}

	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( repeating_statement_with_output )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();

	BOOST_CHECK_NO_THROW(statement.prepare("select nom from paco where id=:id"));
	int i =0;
	string name;
	vector<string> names;
	BOOST_CHECK_NO_THROW(statement.exchange_use(i));
	BOOST_CHECK_NO_THROW(statement.exchange_into(name));
	BOOST_CHECK_NO_THROW(statement.alloc());
	BOOST_CHECK_NO_THROW(statement.define_and_bind());

	// with data exchange
	for(i=0;i<2;++i)
	{
		BOOST_CHECK_NO_THROW(statement.execute(true));;
		names.push_back(name);
	}
	// the two read data are not the same
	BOOST_CHECK(names[0] != names [1]);

	names.clear();
		// with no data exchange
	for(i=0;i<2;++i)
	{
		BOOST_CHECK_NO_THROW(statement.execute(false));
		names.push_back(name);
	}
	// the two read data are the same
	BOOST_CHECK(names[0] == names [1]);

	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}


BOOST_AUTO_TEST_CASE( fetch_statement_with_output )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session=myDatabase->getSingleSession();
	SOCIStatement statement=session.getStatement();

	BOOST_CHECK_NO_THROW(statement.prepare("select nom from paco"));
	string name;
	BOOST_CHECK_NO_THROW(statement.exchange_into(name));
	BOOST_CHECK_NO_THROW(statement.alloc());
	BOOST_CHECK_NO_THROW(statement.define_and_bind());
	// execute with no data exchange for the moment
	BOOST_CHECK_NO_THROW(statement.execute(false));
	// the data exchange is effected by the fetch
	while(statement.fetch())
	{
		BOOST_CHECK(statement.got_data());
	}

	// no more data
	BOOST_CHECK(! statement.got_data());

	myDatabase->releaseSingleSession(session);

	} catch(exception const & e)
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
