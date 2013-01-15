#include <boost/test/unit_test.hpp>
#include "POSIXParser.cpp"
#include "TestParser.cpp"

BOOST_AUTO_TEST_SUITE( test_suite )

BOOST_AUTO_TEST_CASE( my_test_get_next_valeur_n )
{
// Test the posix parser class
  TestPosixParser testPP;
  testPP.testGetNextValeurPrefix();
  testPP.testGetNextValeurKey();
  testPP.testGetNextValeurEqual();
  testPP.testGetNextValeurVal();
  testPP.testGetNextValeurRet();
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_get_next_equalsign_n )
{
// Test the posix parser class
  TestPosixParser testPP;
  testPP.testGetNextEqual();
  testPP.testGetNextEqualSpace();
  testPP.testGetNextEqualMid();
  testPP.testGetNextEqualBad();
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_get_next_token_n )
{
// Test the posix parser class
  TestPosixParser testPP;
  testPP.testGetNextToken();
  testPP.testGetNextTokenNoEq();
  testPP.testGetNextTokenNoDeb();
  testPP.testGetNextTokenEmpty();
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_get_next_line_n )
{
// Test the posix parser class
  TestPosixParser testPP;
  testPP.testGetNextLine();
  testPP.testGetNextLineLast();
  testPP.testGetNextLineLastNoSpace(); // IN ERROR
  testPP.testGetNextLineNoKey();
  testPP.testGetNextLineNoValue();
  testPP.testGetNextLineNoValueSpace();
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_get_next_definition_n )
{
// Test the posix parser class
  TestPosixParser testPP;
  testPP.testGetNextDefinition();
  testPP.testGetNextDefinitionNoKey();
  testPP.testGetNextDefinitionNoVal();
  testPP.testGetNextDefinitionNoEq();
} // boost auto test case

BOOST_AUTO_TEST_SUITE_END()
