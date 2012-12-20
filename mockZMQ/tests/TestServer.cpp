#include <iostream>
#include <vector>
#include "Server.hpp"

class TestServer {

public:
  TestServer(){
    mname = "pierre";
    muri = "tcp://*:5555";
    mservices.push_back("loup");
    mservices.push_back("belette");
  }
  ~TestServer(){
  }

  void
  testFromString(){
    std::string res = mname+"$$$"+muri+"$$$"+mservices.at(0)+"$$$"+mservices.at(1);
    Server s = *(Server::fromString(res).get());
    BOOST_REQUIRE(s.getName().compare(mname) == 0 &&
                s.getURI().compare(muri) == 0 &&
                s.getServices().size() == mservices.size());
  }

  void
  testToString(){
    Server s(mname, mservices, muri);
    std::string res = mname+"$$$"+muri+"$$$"+mservices.at(0)+"$$$"+mservices.at(1);
    BOOST_REQUIRE(res.compare(s.toString()) == 0);
}

  void
  testAdd(){
    std::string str = "lapin";
    Server s(mname, mservices, muri);
    s.add(str);
    BOOST_REQUIRE(s.getServices().size() == 3 &&
                s.hasService(str));
  }

  void
  testRemove(){
    std::string rem = mservices.at(1);
    Server s(mname, mservices, muri);
    s.remove(rem);
    BOOST_REQUIRE(s.getServices().size() == 1 &&
                !s.hasService(rem));
  }

  void
  testRemoveInvalid(){
    std::string rem = "bad";
    int size;
    int res;
    Server s(mname, mservices, muri);
    size = s.getServices().size();
    s.remove(rem);
    res = s.getServices().size();
    BOOST_REQUIRE(res==size);
  }

  void
  testHasService(){
    Server s(mname, mservices, muri);
    BOOST_REQUIRE(s.hasService(mservices.at(0)) &&
                s.hasService(mservices.at(1)));
  }

  void
  testHasServiceBad(){
    Server s(mname, mservices, muri);
    BOOST_REQUIRE(!s.hasService("bad"));
  }

  void
  testGetName(){
    Server s(mname, mservices, muri);
    BOOST_REQUIRE(s.getURI().compare(muri)==0);
  }
  void
  testGetServices(){
    Server s(mname, mservices, muri);
    BOOST_REQUIRE(s.getServices().at(0).compare(mservices.at(0))==0 &&
                s.getServices().at(1).compare(mservices.at(1))==0);
  }

  void
  testGetURI(){
    Server s(mname, mservices, muri);
    BOOST_REQUIRE(s.getURI().compare(muri)==0);
  }

private:
  std::string mname;
  std::vector<std::string> mservices;
  std::string muri;
};


