#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <map>
#include "POSIXParser.hpp"

using namespace std;

static const string TESTSTR="Une ligne\nDeux ligne\n#% vishnu_workingdir    =   /tmp/toto\n\n#% vishnu foireu\n\n#% vishnu_output = /titi\n#% vishnu_error = /titi2  # comments\n#% vishnu_repeat = value1\n#% vishnu_repeat = value2\n";

BOOST_AUTO_TEST_SUITE( POSIXParser_unit_tests )

BOOST_AUTO_TEST_CASE( test_get_n )
{
  map<string, string> presult;
  map<string, string>::const_iterator it;
  stringstream fluxTest(TESTSTR);


  POSIXParser::parseStream(fluxTest, presult);

  BOOST_REQUIRE(presult.find("vishnu_output") != presult.end());
  BOOST_REQUIRE(presult.find("vishnu_workingdir") != presult.end());
  BOOST_REQUIRE_EQUAL(presult["vishnu_workingdir"], "/tmp/toto");
  BOOST_REQUIRE_EQUAL(presult["vishnu_output"], "/titi");
}

BOOST_AUTO_TEST_CASE( test_get_with_comments_n )
{
  map<string, string> presult;
  map<string, string>::const_iterator it;
  stringstream fluxTest(TESTSTR);


  POSIXParser::parseStream(fluxTest, presult);

  BOOST_REQUIRE(presult.find("vishnu_error") != presult.end());
  BOOST_REQUIRE_EQUAL(presult["vishnu_error"], "/titi2");
}

BOOST_AUTO_TEST_CASE( test_get_b )
{
  map<string, string> presult;
  stringstream fluxTest(TESTSTR);

  POSIXParser::parseStream(fluxTest, presult);
  BOOST_REQUIRE(presult.find("no_key") == presult.end());
}

BOOST_AUTO_TEST_CASE( test_get_b2 )
{
  map<string, string> presult;
  stringstream fluxTest(TESTSTR);

  POSIXParser::parseStream(fluxTest, presult);
  BOOST_REQUIRE(presult.find("Une") == presult.end());
  BOOST_REQUIRE(presult.find("Deux") == presult.end());
  BOOST_REQUIRE(presult.find("Une ligne") == presult.end());
  BOOST_REQUIRE(presult.find("Deux ligne") == presult.end());
}

BOOST_AUTO_TEST_CASE( test_get_b3 )
{
  map<string, string> presult;
  stringstream fluxTest(TESTSTR);

  POSIXParser::parseStream(fluxTest, presult);
  BOOST_REQUIRE(presult.find("vishnu") == presult.end());
}

BOOST_AUTO_TEST_CASE( test_twice_inserted_n )
{
  map<string, string> presult;
  stringstream fluxTest(TESTSTR);

  POSIXParser::parseStream(fluxTest, presult);
  BOOST_REQUIRE(presult.find("vishnu_repeat") != presult.end());
  BOOST_REQUIRE_EQUAL(presult["vishnu_repeat"], "value2");
}


BOOST_AUTO_TEST_SUITE_END()
