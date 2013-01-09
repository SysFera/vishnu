#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include "zmq.hpp"
#include "zhelpers.hpp"

static const std::string addr("tcp://localhost:5555");
static zmq::context_t ctxt;


BOOST_AUTO_TEST_SUITE( lazy_pirate_unit_tests )

BOOST_AUTO_TEST_CASE( my_test_send_n )
{
// Test the lazy pirate class
  LazyPirateClient lp(ctxt, addr);
  BOOST_REQUIRE(lp.send("bonjour"));
} // boost auto test case


BOOST_AUTO_TEST_CASE( my_test_send_b_addr )
{
// Test the lazy pirate class
  LazyPirateClient lp (ctxt, "bad");
  BOOST_REQUIRE_THROW(lp.send("bonjour"), std::exception);
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_recv_n )
{
// Test the lazy pirate class
  LazyPirateClient lp (ctxt, addr);
  lp.send("bonjour");
  BOOST_REQUIRE(lp.recv().compare("ok") == 0);
} // boost auto test case

BOOST_AUTO_TEST_CASE( my_test_recv_b_addr )
{
// Test the lazy pirate class
  LazyPirateClient lp (ctxt, "bad");
  BOOST_REQUIRE_THROW(lp.send("bonjour"), std::exception);
  BOOST_REQUIRE(lp.recv().compare("ok") !=0 );
} // boost auto test case

BOOST_AUTO_TEST_SUITE_END()
