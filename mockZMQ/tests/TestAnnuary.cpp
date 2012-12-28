#include <iostream>
#include <vector>
#include <boost/make_shared.hpp>
#include "Annuary.hpp"

class TestAnnuary {
public:
  TestAnnuary(){
    name = "pierre";
    uri = "tcp://*:5555";
    services.push_back("loup");
    services.push_back("belette");

    mservers.push_back(boost::make_shared<Server>(name, services, uri));
  }

  ~TestAnnuary() {}

  void
  testAdd() {
    Annuary ann(mservers);
    std::vector<std::string> servicesTmp;
    servicesTmp.push_back("toto");
    ann.add("titi", "tutu", servicesTmp);
    BOOST_REQUIRE(ann.get("toto").size() == 1);
  }
  void
  testAddTwice() {
    Annuary ann(mservers);
    std::vector<std::string> servicesTmp;
    servicesTmp.push_back("toto");
    ann.add("titi", "tutu", servicesTmp);
    ann.add("titi", "tutu", servicesTmp);
    BOOST_REQUIRE(ann.get().size() == 2);
  }
  void
  testRemove() {
    Annuary ann(mservers);
    ann.remove(name, uri);
    BOOST_REQUIRE(ann.get().empty());
  }
  void
  testRemoveEmpty() {
    Annuary ann(mservers);
    ann.remove(name, uri);
    ann.remove(name, uri);
    BOOST_REQUIRE(ann.get().empty());
  }
  void
  testRemoveBadURI() {
    Annuary ann(mservers);
    ann.remove(name, uri+"1");
    BOOST_REQUIRE(ann.get("loup").size() == 1);
  }
  void
  testRemoveBadName() {
    Annuary ann(mservers);
    ann.remove(name+"1", uri);
    BOOST_REQUIRE(ann.get("loup").size() == 1);
  }
  void
  testGet() {
    Annuary ann(mservers);
    BOOST_REQUIRE(!ann.get("loup").empty() &&
                  !ann.get("belette").empty());
  }
  void
  testGetBad() {
    Annuary ann(mservers);
    BOOST_REQUIRE(ann.get("bad").empty());
  }

private:
  std::vector<boost::shared_ptr<Server> > mservers;
  std::string name;
  std::string uri;
  std::vector<std::string> services;
};
