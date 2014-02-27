#include "SystemException.hpp"

#include <map>                          // for map
#include <utility>                      // for pair


SystemException::SystemException():VishnuException(){initMsg();}

SystemException::SystemException(const SystemException& e):VishnuException(e){initMsg();}

SystemException::SystemException(int msg, std::string msgComp):VishnuException(msg, msgComp){initMsg();}

std::string
SystemException::getTypeS() const{

  std::string errorTypeStr;

  switch (mtype){
  case 0 :
    errorTypeStr="UMS";
    break;
  case 1 :
   errorTypeStr= "TMS";
    break;
  case 2 :
   errorTypeStr= "FMS";
    break;
  case 4 :
   errorTypeStr= "NONE";
    break;
  default :
   errorTypeStr= "->not found<-";
    break;
  }
  return errorTypeStr;
}

void
SystemException::initMsg(){
  mp.insert(std::pair<int, std::string>(ERRCODE_COMMUNICATION, std::string("Communication failed")));
  mp.insert(std::pair<int, std::string>(ERRCODE_DBERR, std::string("Database query failed")));
  mp.insert(std::pair<int, std::string>(ERRCODE_DBCONN, std::string("Database connection failed")));
  mp.insert(std::pair<int, std::string>(ERRCODE_SYSTEM, std::string("VISHNU system failed")));
  mp.insert(std::pair<int, std::string>(ERRCODE_INVCOMP, std::string("Unknown component")));
  mp.insert(std::pair<int, std::string>(ERRCODE_INVMAPPER, std::string("VISHNU Mapper failed")));
  mp.insert(std::pair<int, std::string>(ERRCODE_INVDATA, std::string("Data error")));
  mp.insert(std::pair<int, std::string>(ERRCODE_SSH, std::string("SSH error")));
  mp.insert(std::pair<int, std::string>(ERRCODE_AUTHENTERR, std::string("Authentication error")));
}

std::string
SystemException::getMsg() const{
  if (mp.count(mval)){
    return mp[mval];
  }
  return "Internal Error: Unknown error code.\n";
}
