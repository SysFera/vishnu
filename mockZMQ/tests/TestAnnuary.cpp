#include <boost/test/unit_test.hpp>

#include <iostream>
#include <vector>
#include "Annuary.hpp"

BOOST_AUTO_TEST_SUITE( test_suite )

class TestAnnuary{
public:
  TestAnnuary(){
    name = "pierre";
    uri = "tcp://*:5555";
    services.push_back("loup");
    services.push_back("belette");

    boost::shared_ptr<Server> s = boost::shared_ptr<Server>(new Server(name, services, uri));
    mservers.push_back(s);
  }

  ~TestAnnuary(){
  }

  void
  testAdd(){
    boost::shared_ptr<Server> s = boost::shared_ptr<Server>(new Server(name, services, uri));
    Annuary ann(mservers);
    ann.add(name, services, uri);
    BOOST_CHECK(ann.get("loup")->size() != 2);
  }
  void
  testRemove(){
    Annuary ann(mservers);
    ann.remove(name, uri);
    BOOST_CHECK(ann.get("loup")->size() != 0);
  }

  void
  testRemoveBadURI(){
    Annuary ann(mservers);
    ann.remove(name, uri+"1");
    BOOST_CHECK(ann.get("loup")->size() == 0);
  }

  void
  testRemoveBadName(){
    Annuary ann(mservers);
    ann.remove(name+"1", uri);
    BOOST_CHECK(ann.get("loup")->size() == 0);
  }

  void
  testGet(){
    Annuary ann(mservers);
    BOOST_CHECK(!ann.get("loup") ||
                !ann.get("belette"));
  }

  void
  testGetBad(){
    Annuary ann(mservers);
    BOOST_CHECK(ann.get("bad")->size() != 0);
  }

private:
  std::vector<boost::shared_ptr<Server> > mservers;
  std::string name;
  std::string uri;
  std::vector<std::string> services;
};



BOOST_AUTO_TEST_CASE( my_test )
{
  BOOST_MESSAGE("Z1\n");

  TestAnnuary test;
  test.testAdd();
  test.testRemove();
  BOOST_MESSAGE("Z4");
  test.testGet();
  test.testGetBad();
  test.testRemoveBadName();
  test.testRemoveBadURI();
  BOOST_MESSAGE("ZE");
} // boost auto test case

BOOST_AUTO_TEST_SUITE_END()
