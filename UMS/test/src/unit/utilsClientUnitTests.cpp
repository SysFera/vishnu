#include <boost/test/unit_test.hpp>
#include <string>
#include "utilsClient.hpp"
#include "UserException.hpp"
#include "UMSVishnuException.hpp"

BOOST_AUTO_TEST_SUITE( utilsClient_unit_tests )

BOOST_AUTO_TEST_CASE( test_checkIfTextIsEmpty_b )
{
  std::string empty = "";
  BOOST_REQUIRE_THROW(checkIfTextIsEmpty(empty,
                                         "empty string",
                                         ERRCODE_INVALID_PARAM),
                      UMSVishnuException);
}

BOOST_AUTO_TEST_CASE( test_checkIfTextIsEmpty_n )
{
  std::string nEmpty = "not empty";
  BOOST_REQUIRE_NO_THROW(checkIfTextIsEmpty(nEmpty,
                                            "empty string",
                                            ERRCODE_INVALID_PARAM));
}

BOOST_AUTO_TEST_CASE( test_checkEmail_n )
{
  std::string email1 = "test.toto@yopmail.com";
  std::string email2 = "test.toto.test@univ-somewhere.fr";
  std::string email3 = "contact@someplace.co";
  std::string email4 = "contact@localhost";
  std::string email5 = "contact@univ.co.uk";

  BOOST_REQUIRE_NO_THROW(checkEmail(email1));
  BOOST_REQUIRE_NO_THROW(checkEmail(email2));
  BOOST_REQUIRE_NO_THROW(checkEmail(email3));
  BOOST_REQUIRE_NO_THROW(checkEmail(email4));
  BOOST_REQUIRE_NO_THROW(checkEmail(email5));
}

BOOST_AUTO_TEST_CASE( test_checkEmail_b )
{
  std::string email1 = "test.toto@yopmail.com@somewhere.fr";
  BOOST_REQUIRE_THROW(checkEmail(email1), UMSVishnuException);
}

BOOST_AUTO_TEST_CASE( test_checkEmail_b2 )
{
  std::string email2 = "test.toto.test.univ-somewhere.fr";
  BOOST_REQUIRE_THROW(checkEmail(email2), UMSVishnuException);
}

BOOST_AUTO_TEST_CASE( test_checkEmail_b3 )
{
  std::string email3 = "http://contact@someplace.co";
  BOOST_REQUIRE_THROW(checkEmail(email3), UMSVishnuException);
}

BOOST_AUTO_TEST_CASE( test_checkEmail_b4 )
{
  std::string email3 = "john@aol...com";
  BOOST_REQUIRE_THROW(checkEmail(email3), UMSVishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
