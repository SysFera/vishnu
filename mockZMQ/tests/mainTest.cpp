#include <boost/test/unit_test.hpp>

#include "TestAnnuary.cpp"
#include "TestServer.cpp"
#include "TestLazyPirate.cpp"
#include "TestDIET_client.cpp"

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

// Test the lazy pirate class
  LazyPirateClientTest testlpc;
  testlpc.testSend();
  testlpc.testSendBadAddr();
  testlpc.testRecv();
  testlpc.testRecvBadAddr();

// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testGetModuleUMS();
  testDIET.testGetModuleFMS();
  testDIET.testGetModuleTMS();
  testDIET.testGetModuleIMS();
  testDIET.testGetBadModule();
  testDIET.testDIETAlloc();
  testDIET.testStringSet();
  testDIET.testStringGet();
  testDIET.testDIET_parameterBad();
  testDIET.testDIET_parameter();
  testDIET.testMySerialize();
  testDIET.testMyDeSerialize();
  testDIET.testMyDeSerializeEmpty();
  testDIET.testInitNULL();
  testDIET.testInitInvalid();

} // boost auto test case

BOOST_AUTO_TEST_SUITE_END()
