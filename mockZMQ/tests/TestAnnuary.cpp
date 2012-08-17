#include <iostream>
#include <vector>
#include "Annuary.hpp"

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
    if (ann.get("loup")->size() != 2){
      std::cerr << "Failed add annuary " << std::endl;
    }

  }
  void
  testRemove(){
    Annuary ann(mservers);
    ann.remove(name, uri);
    if (ann.get("loup")->size() != 0){
      std::cerr << "Failed remove annuary " << std::endl;
    }
  }

  void
  testRemoveBadURI(){
    Annuary ann(mservers);
    ann.remove(name, uri+"1");
    if (ann.get("loup")->size() == 0){
      std::cerr << "Failed remove annuary bad uri " << std::endl;
    }
  }

  void
  testRemoveBadName(){
    Annuary ann(mservers);
    ann.remove(name+"1", uri);
    if (ann.get("loup")->size() == 0){
      std::cerr << "Failed remove annuary bad name " << std::endl;
    }
  }

  void
  testGet(){
    Annuary ann(mservers);
    if (!ann.get("loup") ||
        !ann.get("belette")){
      std::cerr << "Failed get annuary " << std::endl;
    }
  }

  void
  testGetBad(){
    Annuary ann(mservers);
    if (ann.get("bad")->size() != 0){
      std::cerr << "Failed get annuary bad getter " << std::endl;
    }
  }

private:
  std::vector<boost::shared_ptr<Server> > mservers;
  std::string name;
  std::string uri;
  std::vector<std::string> services;
};

int main(int argc, char** argv){
  TestAnnuary test;
  test.testAdd();
  test.testRemove();
  test.testGet();
  test.testGetBad();
  test.testRemoveBadName();
  test.testRemoveBadURI();
  return 0;
}
