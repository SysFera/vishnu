#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <map>
#include "utilServer.hpp"

BOOST_AUTO_TEST_SUITE( utilServer_unit_tests )

BOOST_AUTO_TEST_CASE( test_validateUri_n )
{

  std::string input_1 = "tcp://toto";
  std::string input_2 = "tcp://*";

  vishnu::validateUri(input_1);
  BOOST_REQUIRE_THROW(vishnu::validateUri(input_2), VishnuException);
  BOOST_MESSAGE("Test validate URI OK");
}

BOOST_AUTO_TEST_CASE( test_isMonth_n )
{

  std::string input_1 = "MONTH";
  std::string input_2 = "month";
  std::string input_3 = "isMONTH";
  std::string input_4 = "MONTH2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isMonth(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isMonth(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isMonth(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isMonth(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isMonth(input_5.c_str()));
  BOOST_MESSAGE("Test is month OK");
}

BOOST_AUTO_TEST_CASE( test_isYear_n )
{

  std::string input_1 = "YEAR";
  std::string input_2 = "year";
  std::string input_3 = "isYEAR";
  std::string input_4 = "YEAR2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isYear(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isYear(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isYear(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isYear(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isYear(input_5.c_str()));
  BOOST_MESSAGE("Test is year OK");
}

BOOST_AUTO_TEST_CASE( test_isSite_n )
{

  std::string input_1 = "SITE";
  std::string input_2 = "site";
  std::string input_3 = "isSITE";
  std::string input_4 = "SITE2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isSite(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isSite(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isSite(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isSite(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isSite(input_5.c_str()));
  BOOST_MESSAGE("Test is site OK");
}

BOOST_AUTO_TEST_CASE( test_isType_n )
{

  std::string input_1 = "TYPE";
  std::string input_2 = "type";
  std::string input_3 = "isTYPE";
  std::string input_4 = "TYPE2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isType(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isType(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isType(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isType(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isType(input_5.c_str()));
  BOOST_MESSAGE("Test is type OK");
}

BOOST_AUTO_TEST_CASE( test_isUName_n )
{

  std::string input_1 = "UNAME";
  std::string input_2 = "uname";
  std::string input_3 = "isUNAME";
  std::string input_4 = "UNAME2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isUName(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isUName(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isUName(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isUName(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isUName(input_5.c_str()));
  BOOST_MESSAGE("Test is uname OK");
}

BOOST_AUTO_TEST_CASE( test_isMAName_n )
{

  std::string input_1 = "MANAME";
  std::string input_2 = "uname";
  std::string input_3 = "isMANAME";
  std::string input_4 = "MANAME2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isMaName(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isMaName(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isMaName(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isMaName(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isMaName(input_5.c_str()));
  BOOST_MESSAGE("Test is maname OK");
}

BOOST_AUTO_TEST_CASE( test_isDay_n )
{

  std::string input_1 = "DAY";
  std::string input_2 = "day";
  std::string input_3 = "isDAY";
  std::string input_4 = "DAY2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isDay(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isDay(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isDay(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isDay(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isDay(input_5.c_str()));
  BOOST_MESSAGE("Test is day OK");
}

BOOST_AUTO_TEST_CASE( test_isCpt_n )
{

  std::string input_1 = "CPT";
  std::string input_2 = "cpt";
  std::string input_3 = "isCPT";
  std::string input_4 = "CPT2";
  std::string input_5 = "lapin";

  BOOST_REQUIRE(vishnu::isCpt(input_1.c_str()));
  BOOST_REQUIRE(!vishnu::isCpt(input_2.c_str()));
  BOOST_REQUIRE(!vishnu::isCpt(input_3.c_str()));
  BOOST_REQUIRE(vishnu::isCpt(input_4.c_str()));
  BOOST_REQUIRE(!vishnu::isCpt(input_5.c_str()));
  BOOST_MESSAGE("Test is cpt OK");
}


BOOST_AUTO_TEST_CASE( test_getKeywords_n )
{

  std::string res = "U_1_JOB_lapin_garenne";
  std::string format = "U_$CPT_$TYPE_$UNAME_$SITE";
  int cpt = 1;
  vishnu::IdType type (vishnu::JOB);
  std::string name = "lapin";
  std::string site = "garenne";
  int  size;
  Format_t *keywords;
  keywords = new Format_t[strlen(format.c_str())+1];

  BOOST_REQUIRE(getKeywords (&size, keywords, format.c_str(), cpt, type, name, site) != -1);
  BOOST_REQUIRE(keywords[0].value=="1");
  BOOST_REQUIRE(keywords[1].value=="J");
  BOOST_REQUIRE(keywords[2].value==name);
  BOOST_REQUIRE(keywords[3].value==site);

  BOOST_MESSAGE("Test get keywords OK");
}


BOOST_AUTO_TEST_CASE( test_getGeneratedName_n )
{

  std::string res_1 = "U_1_J_lapin_garenne";
  std::string res_2 = "U_1_J";
  std::string res_3 = "U_1_garenne";
  std::string res_4 = "1_J_lapin_garenne";
  std::string format_1 = "U_$CPT_$TYPE_$UNAME_$SITE";
  std::string format_2 = "U_$CPT_$TYPE";
  std::string format_3 = "U_$CPT_$SITE";
  std::string format_4 = "$CPT_$TYPE_$UNAME_$SITE";
  int cpt = 1;
  vishnu::IdType type (vishnu::JOB);
  std::string name = "lapin";
  std::string site = "garenne";

  BOOST_REQUIRE(res_1 == vishnu::getGeneratedName(format_1.c_str(), cpt, type, name, site));
  BOOST_REQUIRE(res_2 == vishnu::getGeneratedName(format_2.c_str(), cpt, type, name, site));
  BOOST_REQUIRE(res_3 == vishnu::getGeneratedName(format_3.c_str(), cpt, type, name, site));
  BOOST_REQUIRE(res_4 == vishnu::getGeneratedName(format_4.c_str(), cpt, type, name, site));
  BOOST_MESSAGE("Test get name OK");
}


BOOST_AUTO_TEST_SUITE_END()
