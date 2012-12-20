#include <boost/test/unit_test.hpp>

#include "TestAnnuary.cpp"
#include "TestServer.cpp"
#include "TestLazyPirate.cpp"
#include "TestDIET_client.cpp"

BOOST_AUTO_TEST_SUITE( test_suite )


BOOST_AUTO_TEST_CASE( my_test_add_n )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testAdd();
} // boost auto test case


BOOST_AUTO_TEST_CASE( my_test_remove_n )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testRemove();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_get_n )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testGet();
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_get_b )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testGetBad();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_remove_b_nane )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testRemoveBadName();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_remove_b_uri )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testRemoveBadURI();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_add_b_twice )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testAddTwice();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_remove_b_empty )
{
// Test the annuary class
  TestAnnuary testAnn;
  testAnn.testRemoveEmpty();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_fromStr_n )
{
// Test the server class
  TestServer testSer;
  testSer.testFromString();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_toStr_n )
{
// Test the server class
  TestServer testSer;
  testSer.testToString();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_add_n_serv )
{
// Test the server class
  TestServer testSer;
  testSer.testAdd();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_remove_n_serv )
{
// Test the server class
  TestServer testSer;
  testSer.testRemove();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_remove_b_invalid )
{
// Test the server class
  TestServer testSer;
  testSer.testRemoveInvalid();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_haz_n_ser )
{
// Test the server class
  TestServer testSer;
  testSer.testHasService();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_haz_b )
{
// Test the server class
  TestServer testSer;
  testSer.testHasServiceBad();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_getname_n )
{
// Test the server class
  TestServer testSer;
  testSer.testGetName();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_getserv_n )
{
// Test the server class
  TestServer testSer;
  testSer.testGetServices();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_geturi_n )
{
// Test the server class
  TestServer testSer;
  testSer.testGetURI();
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_send_n )
{
// Test the lazy pirate class
  LazyPirateClientTest testlpc;
  testlpc.testSend();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_send_b_addr )
{
// Test the lazy pirate class
  LazyPirateClientTest testlpc;
  testlpc.testSendBadAddr();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_recv_n )
{
// Test the lazy pirate class
  LazyPirateClientTest testlpc;
  testlpc.testRecv();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_recv_b_addr )
{
// Test the lazy pirate class
  LazyPirateClientTest testlpc;
  testlpc.testRecvBadAddr();
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_get_n_ums )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testGetModuleUMS();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_get_n_fms )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testGetModuleFMS();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_get_n_tms )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testGetModuleTMS();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_get_n_ims )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testGetModuleIMS();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_get_b_mod )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testGetBadModule();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_alloc_n )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testDIETAlloc();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_setStr_n )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testStringSet();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_getStr_n )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testStringGet();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_param_b )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testDIET_parameterBad();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_parameter_n )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testDIET_parameter();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_serial_n )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testMySerialize();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_deser_n )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testMyDeSerialize();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_deser_b_emp )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testMyDeSerializeEmpty();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_init_b_nul )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testInitNULL();
} // boost auto test case
BOOST_AUTO_TEST_CASE( my_test_init_b_invalid )
{
// Test the DIET client overload
  TestDIET_client testDIET;
  testDIET.testInitInvalid();
} // boost auto test case

BOOST_AUTO_TEST_SUITE_END()
