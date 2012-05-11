#include "Annuary.hpp"
#include "mdcliapi.hpp"
#include <string>
#include <boost/shared_ptr.hpp>


Annuary::Annuary(){
}

Annuary::~Annuary(){
//  int i;
//  for (i=0;i<mservers.size();++i){
//    delete(mservers.at(i));
//  }
}


int
Annuary::add(std::string name, std::vector<std::string> services, int port, std::string address){
  boost::shared_ptr<Server> s;
  s.reset (new Server(name, services, address, port));
  mservers.push_back(s);
  return 0;
}


int
Annuary::remove(std::string name, int port, std::string address){
  int i;
  for (i=0;i<mservers.size();++i){
    if (name.compare(mservers.at(i)->getName()) == 0 &&
        address.compare(mservers.at(i).get()->getAddress()) == 0 &&
        port == mservers.at(i).get()->getPort()){
      mservers.erase(mservers.begin()+i);
    }
  }
  return 0;
}


boost::shared_ptr<Server>
Annuary::get(std::string service){
  int i;
  for (i=0;i<mservers.size();++i){
    if (mservers.at(i)->hasService(service)){
      return mservers.at(i);
    }
  }
  return boost::shared_ptr<Server>();
}



