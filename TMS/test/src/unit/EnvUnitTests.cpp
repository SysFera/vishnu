#include <boost/test/unit_test.hpp>
#include <string>
#include <boost/format.hpp>
#include <cstdlib>
#include "TMSVishnuException.hpp"

#include "tmsUtils.hpp"

std::string jobId = "$VISHNU_BATCHJOB_ID";
std::string jobId2 = "${VISHNU_BATCHJOB_ID}";
std::string jobName = "$VISHNU_BATCHJOB_NAME";
std::string jobName2 = "${VISHNU_BATCHJOB_NAME}";
std::string jobNodeFile = "$VISHNU_BATCHJOB_NODEFILE";
std::string jobNodeFile2 = "${VISHNU_BATCHJOB_NODEFILE}";
std::string numNodes = "$VISHNU_BATCHJOB_NUM_NODES";
std::string numNodes2 = "${VISHNU_BATCHJOB_NUM_NODES}";

BOOST_AUTO_TEST_SUITE( env_unit_tests )

BOOST_AUTO_TEST_CASE( test_replaceAllOccurences_n )
{
  std::string script = "SOLDIER #1:\nYou've got two empty halves of coconut and you're bangin' 'em together.\nARTHUR:\nSo? We have ridden since the snows of winter covered this land, through the kingdom of Mercia, through--\nSOLDIER #1:\nWhere'd you get the coconuts?\nARTHUR:\nWe found them.";

  std::string scriptModif = "SOLDIER #1:\nYou've got two empty halves of apple and you're bangin' 'em together.\nARTHUR:\nSo? We have ridden since the snows of winter covered this land, through the kingdom of Mercia, through--\nSOLDIER #1:\nWhere'd you get the apples?\nARTHUR:\nWe found them.";

  vishnu::replaceAllOccurences(script, "coconut", "apple");

  BOOST_REQUIRE_EQUAL(script, scriptModif);
}


BOOST_AUTO_TEST_CASE( test_getVar_n )
{
  std::string envVar = "AZERTYUIOPWXCVBNQSDFGHJKL";
  std::string value = "pouet";
  std::string defaultValue = "toto";
  std::string val;

  unsetenv(envVar.c_str());

  BOOST_REQUIRE_NO_THROW((val = vishnu::getVar(envVar, true, defaultValue)));
  BOOST_REQUIRE_EQUAL(val, defaultValue);

  setenv(envVar.c_str(), value.c_str(), 1);
  BOOST_REQUIRE_NO_THROW((val = vishnu::getVar(envVar, true, defaultValue)));
  BOOST_REQUIRE_EQUAL(val, value);

  BOOST_REQUIRE_NO_THROW((val = vishnu::getVar(envVar, false, defaultValue)));
  BOOST_REQUIRE_EQUAL(val, value);

  BOOST_REQUIRE_NO_THROW((val = vishnu::getVar(envVar, false)));
  BOOST_REQUIRE_EQUAL(val, value);

  unsetenv(envVar.c_str());
}


BOOST_AUTO_TEST_CASE( test_getVar_b )
{
  std::string envVar = "AZERTYUIOPWXCVBNQSDFGHJKL";
  std::string value = "pouet";
  std::string defaultValue = "toto";
  std::string val;

  unsetenv(envVar.c_str());

  BOOST_REQUIRE_THROW((val = vishnu::getVar(envVar, false, defaultValue)), TMSVishnuException);
}



BOOST_AUTO_TEST_SUITE_END()
