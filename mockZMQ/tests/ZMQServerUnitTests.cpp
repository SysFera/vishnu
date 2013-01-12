#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include "Server.hpp"

std::string mname;
std::vector<std::string> mservices;
std::string muri;

class ZMQServerFixture {
public:
  ZMQServerFixture(){
    mname = "pierre";
    muri = "tcp://*:5555";
    mservices.clear();
    mservices.push_back("loup");
    mservices.push_back("belette");
  }

  ~ZMQServerFixture(){
    mservices.clear();
  }
};


BOOST_FIXTURE_TEST_SUITE( ZMQserver_unit_tests, ZMQServerFixture )


BOOST_AUTO_TEST_CASE( test_fromStr_n )
{
// Test the server class
  std::string res = mname + "$$$" + muri + "$$$" + mservices.at(0) + "$$$" + mservices.at(1);
  boost::shared_ptr<Server> s = Server::fromString(res);

  BOOST_REQUIRE_EQUAL(s->getName(), mname);
  BOOST_REQUIRE_EQUAL(s->getURI(), muri);
  BOOST_REQUIRE_EQUAL((s->getServices()).size(), mservices.size());
}


BOOST_AUTO_TEST_CASE( test_toStr_n )
{
// Test the server class
  Server s(mname, mservices, muri);
  std::string res = mname + "$$$" + muri + "$$$" + mservices.at(0) + "$$$" + mservices.at(1);

  BOOST_REQUIRE_EQUAL(res, s.toString());
}

BOOST_AUTO_TEST_CASE( test_add_n_serv )
{
// Test the server class
  std::string str = "lapin";
  Server s(mname, mservices, muri);
  s.add(str);

  BOOST_REQUIRE_EQUAL(s.getServices().size(), 3);
  BOOST_REQUIRE(s.hasService(str));
}

BOOST_AUTO_TEST_CASE( test_add_twice_n_serv )
{
// Test the server class
  std::string str = "lapin";
  Server s(mname, mservices, muri);
  s.add(str);
  s.add(str);

  BOOST_REQUIRE_EQUAL(s.getServices().size(), 3);
  BOOST_REQUIRE(s.hasService(str));
}

BOOST_AUTO_TEST_CASE( test_remove_n_serv )
{
// Test the server class
  std::string rem = mservices.at(1);
  Server s(mname, mservices, muri);
  s.remove(rem);

  BOOST_REQUIRE_EQUAL(s.getServices().size(), 1);
  BOOST_REQUIRE(!s.hasService(rem));
}

BOOST_AUTO_TEST_CASE( test_remove_b_invalid )
{
// Test the server class
  std::string rem = "bad";
  int size;
  int res;
  Server s(mname, mservices, muri);
  size = s.getServices().size();
  s.remove(rem);
  res = s.getServices().size();

  BOOST_REQUIRE_EQUAL(res, size);
}

BOOST_AUTO_TEST_CASE( test_haz_n_ser )
{
// Test the server class
  Server s(mname, mservices, muri);

  BOOST_REQUIRE(s.hasService(mservices.at(0)));
  BOOST_REQUIRE(s.hasService(mservices.at(1)));
}

BOOST_AUTO_TEST_CASE( test_haz_b )
{
// Test the server class
  Server s(mname, mservices, muri);

  BOOST_REQUIRE(!s.hasService("bad"));
}

BOOST_AUTO_TEST_CASE( test_getname_n )
{
// Test the server class
  Server s(mname, mservices, muri);

  BOOST_REQUIRE_EQUAL(s.getURI(), muri);
}

BOOST_AUTO_TEST_CASE( test_getserv_n )
{
// Test the server class
  Server s(mname, mservices, muri);

  BOOST_REQUIRE_EQUAL(s.getServices().at(0), mservices.at(0));
  BOOST_REQUIRE_EQUAL(s.getServices().at(1), mservices.at(1));
}

BOOST_AUTO_TEST_CASE( test_geturi_n )
{
// Test the server class
  Server s(mname, mservices, muri);

  BOOST_REQUIRE_EQUAL(s.getURI(), muri);
}


BOOST_AUTO_TEST_SUITE_END()
