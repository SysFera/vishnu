#include "ObjectIdServer.hpp"

#include <string>
#include <boost/algorithm/string.hpp>

#include "DbFactory.hpp"

ObjectIdServer::ObjectIdServer(const UserServer session):msession(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

ObjectIdServer::~ObjectIdServer() {
}


bool
ObjectIdServer::containCpt(std::string fmt) {
  return (fmt.find("$CPT")!=std::string::npos);
}
