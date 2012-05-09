#include "Server.cpp"
#include <vector>
#include <string>

Server::Server(int name, std::vector<std::string> &serv, std::string addr, int port) :
    mname(name), mservices(serv), maddress(addr), mport(port) {
}

Server::~Server(){}


int
Server::add(std::string service){
  bool found = false;
  int i;
  for (i=0;i<services.size();++i){
    if (service.compare(services.at(i)==0)){
      found = true;
    }
  }
  if (!found){
    services.push_pach(service);
  }
}

int
Server::remove(std::string service){
  int i;
  for (i=0;i<services.size();++i){
    if (service.compare(services.at(i)==0)){
      services.erase(services.begin()+i);
    }
  }
}

bool
Server::hasService(std::string service){
  int i;
  for (i=0;i<services.size();++i){
    if (service.compare(services.at(i)==0)){
      return true;
    }
  }
  return false;
}

std::string
Server::getName(){
  return mname;
}

int
Server::getPort(){
  return mport;
}

std::string
Server::getAddress(){
  return maddress;
}

std::string
Server::ToString(){
  std::stringstream res;
  int i;
  res << mname << "$$$"
      << madress << "$$$"
      << mport << "$$$";
  for (i=0 ; i<mservices.size()-1 ; ++i){
    res << mservices.at(i) << "$$$";
  }
  res << mservices.at(mservices.size()-1)
  retrun res.str();
}

static boost::shared_ptr<Server>
Server::fromString(std::string){
  boost::shared_ptr<Server> res;
  std::vector<std::string> vecString;
  boost::algorithm::split_regex(vecString, prof, boost::regex("\\${3}"));
  std::string name;
  std::string port;
  std::string address;
  std::vector<std::string> services;
  int i;

  if (!vecString.empty()) {
    std::vector<std::string>::iterator it= vecString.begin();
    name = std::string(strdup((it++)->c_str()));
    address = std::string(strdup((it++)->c_str()));
    port = boost::lexical_cast<int>(*(it++));
    for (i=0; it != vecString.end(); it++,i++){
      services.at(i) = std::string(strdup(it->c_str()));
    }
    res.reset(new Server(name, services, address, port));
  }
  return res;
}

