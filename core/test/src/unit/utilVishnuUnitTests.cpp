#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <map>

#include "UserException.hpp"
#include "utilVishnu.hpp"

BOOST_AUTO_TEST_SUITE( utilVishnu_unit_tests )

BOOST_AUTO_TEST_CASE( test_convertToString_n )
{

  std::string res = "12345";
  int  input_i = 12345;
  long input_l = 12345;

  BOOST_REQUIRE(vishnu::convertToString(input_l) == res);
  BOOST_REQUIRE(vishnu::convertToString(input_i) == res);
  BOOST_MESSAGE("Test convert to string OK");
}

BOOST_AUTO_TEST_CASE( test_convertToInt_n )
{

  std::string input = "12345";
  int res = 12345;

  BOOST_REQUIRE(vishnu::convertToInt(input) == res);
  BOOST_MESSAGE("Test convert to int OK");
}

BOOST_AUTO_TEST_CASE( test_convertToInt_b )
{

  std::string input = "pouet";

  BOOST_REQUIRE_NO_THROW(vishnu::convertToInt(input));
  BOOST_REQUIRE_EQUAL(vishnu::convertToInt(input), -1);
  BOOST_MESSAGE("Test convert to int OK");
}

BOOST_AUTO_TEST_CASE( test_convertToLong_n )
{

  std::string input = "12345";
  long res = 12345;

  BOOST_REQUIRE(vishnu::convertToLong(input) == res);
  BOOST_MESSAGE("Test convert to long OK");
}

BOOST_AUTO_TEST_CASE( test_convertToLong_b )
{

  std::string input = "pouet";

  BOOST_REQUIRE_NO_THROW(vishnu::convertToLong(input));
  BOOST_REQUIRE_EQUAL(vishnu::convertToLong(input), -1);
  BOOST_MESSAGE("Test convert to long OK");
}


BOOST_AUTO_TEST_CASE( test_cryptPwd_n )
{

  std::string user = "root";
  std::string pwd  = "vishnu_user";
  std::string res = "8vU7h/n6KOW8reLF1Lt2/5gzjZ.HvGK3A9doVMbmPtaYKkkCoWrMKiPa7s.fEigSTS5gQmX5F8BlW2XotCeHa0";

  BOOST_REQUIRE(vishnu::cryptPassword(user, pwd) == res);
  BOOST_REQUIRE(vishnu::cryptPassword(user, pwd, true) == res);
  BOOST_REQUIRE(vishnu::cryptPassword(user, pwd, false) == pwd);
  BOOST_MESSAGE("Test cryptPwd OK");
}


BOOST_AUTO_TEST_CASE( test_isNumerical_n )
{
  std::string input_1 = "12345";
  std::string input_2 = "lapin";
  std::string input_3 = "james0";
  std::string input_4 = "";

  BOOST_REQUIRE(vishnu::isNumericalValue(input_1));
  BOOST_REQUIRE_THROW(vishnu::isNumericalValue(input_2), UserException);
  BOOST_REQUIRE_THROW(vishnu::isNumericalValue(input_3), UserException);
  BOOST_REQUIRE_THROW(vishnu::isNumericalValue(input_4), UserException);
  BOOST_MESSAGE("Test isNumerical OK");
}

BOOST_AUTO_TEST_CASE( test_checkMetricHistVal_n )
{
  int input_1 = 1;
  int input_2 = -2;
  int input_3 = 8;

  vishnu::checkMetricHistoryValue(input_1);
  BOOST_REQUIRE_THROW(vishnu::checkMetricHistoryValue(input_2), UserException);
  BOOST_REQUIRE_THROW(vishnu::checkMetricHistoryValue(input_3), UserException);
  BOOST_MESSAGE("Test check metric history value OK");
}

BOOST_AUTO_TEST_CASE( test_remotePath_n )
{
  std::string input_1 = "host:/path";
  std::string input_2 = "/345-4";
  std::string input_3 = "/lapin/tmp";

  vishnu::checkRemotePath(input_1);
  BOOST_REQUIRE_THROW(vishnu::checkRemotePath(input_2), VishnuException);
  BOOST_REQUIRE_THROW(vishnu::checkRemotePath(input_3), VishnuException);
  BOOST_MESSAGE("Test check remote path OK");
}


BOOST_AUTO_TEST_CASE( test_emptyString_n )
{
  std::string input_1 = "";
  std::string input_2 = " ";
  std::string input_3 = "blabla";

  BOOST_REQUIRE_THROW(vishnu::checkEmptyString(input_1, "fail"), UserException);
  vishnu::checkEmptyString(input_2, "ok");
  vishnu::checkEmptyString(input_3, "ok");
  BOOST_MESSAGE("Test check empty string OK");
}

BOOST_AUTO_TEST_CASE( test_isNotIP_nv4 )
{
  // IPv4
  BOOST_REQUIRE(!vishnu::isNotIP("127.0.0.1"));
  BOOST_REQUIRE(!vishnu::isNotIP("192.168.0.12"));
  BOOST_REQUIRE(!vishnu::isNotIP("1.2.3.4"));
}

BOOST_AUTO_TEST_CASE( test_isNotIP_nv6 )
{
  // IPv6
  BOOST_REQUIRE(!vishnu::isNotIP("fe80::fa1e:dfff:feec:2b2d"));
  BOOST_REQUIRE(!vishnu::isNotIP("fe80::200:ff:fe00:0"));
  BOOST_REQUIRE(!vishnu::isNotIP("fe80::224:e8ff:fe3c:f559"));
}

BOOST_AUTO_TEST_CASE( test_isNotIP_b )
{
  // IPv6
  BOOST_REQUIRE(vishnu::isNotIP("localhost"));
  BOOST_REQUIRE(vishnu::isNotIP("toto"));
  BOOST_REQUIRE(vishnu::isNotIP("127.0.1.1.1"));
  BOOST_REQUIRE(vishnu::isNotIP("127.0.a.b"));
}


BOOST_AUTO_TEST_SUITE_END()
