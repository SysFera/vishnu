#include "Server.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>

// typedef for readability and less typing
typedef std::vector<std::string> Services;


Server::Server(const std::string& name, const std::vector<std::string> &serv,
               const std::string& uri) :
    mname(name), mservices(serv), muri(uri) {
}

Server::Server(Server& serv){
  mname = serv.getName();
  mservices = serv.getServices();
  muri = serv.getURI();
}

Server::~Server(){
}


int
Server::add(const std::string& service) {
  Services::iterator it =
    std::find(mservices.begin(), mservices.end(), service);

  if (it == mservices.end()) {
    mservices.push_back(service);
  }

  return 0;
}

int
Server::remove(const std::string& service) {
  // using erase/remove idiom
  Services::iterator it =
    std::remove(mservices.begin(), mservices.end(), service);

  mservices.erase(it, mservices.end());

  return 0;
}

bool
Server::hasService(const std::string& service) const {
  Services::const_iterator it  =
    std::find(mservices.begin(), mservices.end(), service);

  return (it != mservices.end());
}

std::string
Server::getName() const {
  return mname;
}


std::string
Server::getURI() const {
  return muri;
}

Services&
Server::getServices() {
  return mservices;
}


std::string
Server::toString() {
  std::stringstream res;
  unsigned int i;
  res << mname << "$$$"
      << muri << "$$$";
  for (i = 0 ; i < mservices.size() - 1 ; ++i){
    res << mservices.at(i) << "$$$";
  }
  res << mservices.at(mservices.size()-1) << "$$$";
  return res.str();
}

boost::shared_ptr<Server>
Server::fromString(const std::string& prof) {
  using boost::algorithm::split_regex;

  boost::shared_ptr<Server> res;
  std::vector<std::string> vecString;
  split_regex(vecString, prof, boost::regex("\\${3}"));
  std::string name;
  std::string uri;
  Services services;

  if (!vecString.empty()) {
    Services::iterator it = vecString.begin();
    name = *(it++);
    uri = *(it++);

    // we only copy non-empty strings to avoid deserialization errors
    std::remove_copy_if(it, vecString.end(),
                        std::back_inserter(services),
                        boost::bind(&std::string::empty, _1));

    res = boost::make_shared<Server>(name, services, uri);
  }
  return res;
}
