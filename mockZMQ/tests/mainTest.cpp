#include <boost/test/unit_test.hpp>

#include "TestAnnuary.cpp"
#include "TestServer.cpp"
#include "TestLazyPirate.cpp"

BOOST_AUTO_TEST_SUITE( test_suite )


BOOST_AUTO_TEST_CASE( my_test )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testAdd();
  testAnn.testRemove();
  testAnn.testGet();
  testAnn.testGetBad();
  testAnn.testRemoveBadName();
  testAnn.testRemoveBadURI();
  testAnn.testAddTwice();
  testAnn.testRemoveEmpty();

// Test the server class
  TestServer testSer;
  testSer.testFromString();
  testSer.testToString();
  testSer.testAdd();
  testSer.testRemove();
  testSer.testRemoveInvalid();
  testSer.testHasService();
  testSer.testHasServiceBad();
  testSer.testGetName();
  testSer.testGetServices();
  testSer.testGetURI();

  LazyPirateClientTest test;
  test.testSend();
  test.testSendBadAddr();
  test.testRecv();
  test.testRecvBadAddr();

} // boost auto test case

BOOST_AUTO_TEST_SUITE_END()
