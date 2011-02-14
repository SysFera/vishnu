#include "utilServer.hh"
#include "UMSVishnuException.hh"


std::string Vishnuid::mvishnuid = "";
//std::string utilServer::mvishnuid = "";

int utilServer::convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val); 
  str >> intValue;
  return static_cast<int> (intValue);
}

int utilServer::checkId(int id) {
  if (id < 0) {
    UMSVishnuException e (4, "The id of the object is incorrect");
    throw e;
  }
  return id;
}
