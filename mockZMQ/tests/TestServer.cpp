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
    if (s.getName().compare(mname) != 0 ||
        s.getURI().compare(muri) != 0 ||
        s.getServices().size() != mservices.size()){
          std::cerr << "Failed fromString" << std::endl;
    }

  }

  void
  testToString(){
    Server s(mname, mservices, muri);
    std::string res = mname+"$$$"+muri+"$$$"+mservices.at(0)+"$$$"+mservices.at(1);
    if (res.compare(s.toString()) != 0){
          std::cerr << "Failed toString" << std::endl;
    }

}

  void
  testAdd(){
    std::string str = "lapin";
    Server s(mname, mservices, muri);
    s.add(str);
    if (s.getServices().size() != 3 ||
        !s.hasService(str)){
          std::cerr << "Failed add" << std::endl;
    }
  }

  void
  testRemove(){
    std::string rem = mservices.at(1);
    Server s(mname, mservices, muri);
    s.remove(rem);
    if (s.getServices().size() != 1 ||
        s.hasService(rem)){
          std::cerr << "Failed remove" << std::endl;
    }
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
    if (res != size){
          std::cerr << "Failed remove invalid entry" << std::endl;
    }
  }

  void
  testHasService(){
    Server s(mname, mservices, muri);
    if (!s.hasService(mservices.at(0)) ||
        !s.hasService(mservices.at(1))){
      std::cerr << "Failed hasService" << std::endl;
    }
  }

  void
  testHasServiceBad(){
    Server s(mname, mservices, muri);
    if (s.hasService("bad")){
      std::cerr << "Failed hasService bad entry" << std::endl;
    }
  }

  void
  testGetName(){
    Server s(mname, mservices, muri);
    if(s.getURI().compare(muri)!=0){
      std::cerr << "Failed get URI" << std::endl;
    }
  }
  void
  testGetServices(){
    Server s(mname, mservices, muri);
    if(s.getServices().at(0).compare(mservices.at(0))!=0 ||
       s.getServices().at(1).compare(mservices.at(1))!=0){
      std::cerr << "Failed get Services" << std::endl;
    }
  }

  void
  testGetURI(){
    Server s(mname, mservices, muri);
    if(s.getURI().compare(muri)!=0){
      std::cerr << "Failed get URI" << std::endl;
    }
  }

private:
  std::string mname;
  std::vector<std::string> mservices;
  std::string muri;
};





int main (int argc, char** argv){

  TestServer test;
  test.testGetURI();
  test.testGetName();
  test.testGetServices();
  test.testHasService();
  test.testAdd();
  test.testRemove();
  test.testFromString();
  test.testToString();
  test.testHasServiceBad();
  test.testRemoveInvalid();
  return 0;
}
