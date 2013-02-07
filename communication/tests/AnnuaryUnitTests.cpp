#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include <boost/make_shared.hpp>
#include "Annuary.hpp"

std::vector<boost::shared_ptr<Server> > mservers;
std::string name = "pierre";
std::string uri = "tcp://*:5555";
std::vector<std::string> services;


class AnnuaryFixture {
public:
  AnnuaryFixture(){
    name = "pierre";
    uri = "tcp://*:5555";
    services.clear();
    services.push_back("loup");
    services.push_back("belette");

    mservers.clear();
    mservers.push_back(boost::make_shared<Server>(name, services, uri));
  }

  ~AnnuaryFixture(){
    mservers.clear();
    services.clear();
  }
};




BOOST_FIXTURE_TEST_SUITE( annuary_unit_tests, AnnuaryFixture )


BOOST_AUTO_TEST_CASE( test_default_constructor_n )
{
  Annuary ann;
  BOOST_REQUIRE_EQUAL(ann.get().size(), 0);
}


BOOST_AUTO_TEST_CASE( test_add_n )
{
// Test the annuary class
  Annuary ann(mservers);
  std::vector<std::string> servicesTmp;
  servicesTmp.push_back("toto");
  ann.add("titi", "tutu", servicesTmp);
  BOOST_REQUIRE_EQUAL(ann.get("toto").size(), 1);
}


BOOST_AUTO_TEST_CASE( test_remove_n )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name, uri);
  BOOST_REQUIRE(ann.get().empty());
}

BOOST_AUTO_TEST_CASE( test_get_n )
{
// Test the annuary class
  Annuary ann(mservers);
  BOOST_REQUIRE(!ann.get("loup").empty());
  BOOST_REQUIRE(!ann.get("belette").empty());
}

BOOST_AUTO_TEST_CASE( test_get_bad )
{
// Test the annuary class
  Annuary ann(mservers);
  BOOST_REQUIRE(ann.get("bad").empty());
}

BOOST_AUTO_TEST_CASE( test_remove_bad_name )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name+"1", uri);
  BOOST_REQUIRE(ann.get("loup").size() == 1);
}

BOOST_AUTO_TEST_CASE( test_remove_bad_uri )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name, uri+"1");
  BOOST_REQUIRE(ann.get("loup").size() == 1);
}

BOOST_AUTO_TEST_CASE( test_add_bad_twice )
{
// Test the annuary class
  Annuary ann(mservers);
  std::vector<std::string> servicesTmp;
  servicesTmp.push_back("toto");
  ann.add("titi", "tutu", servicesTmp);
  ann.add("titi", "tutu", servicesTmp);
  BOOST_REQUIRE(ann.get().size() == 2);
}

BOOST_AUTO_TEST_CASE( test_remove_bad_empty )
{
// Test the annuary class
  Annuary ann(mservers);
  ann.remove(name, uri);
  ann.remove(name, uri);
  BOOST_REQUIRE(ann.get().empty());
}

BOOST_AUTO_TEST_SUITE_END()
