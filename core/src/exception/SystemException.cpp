#include "SystemException.hpp"


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
  case 3 :
   errorTypeStr= "IMS";
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
  mp.insert(std::pair<int, std::string>(ERRCODE_DIET, std::string("Vishnu not available (Service bus failure)")));
  mp.insert(std::pair<int, std::string>(ERRCODE_DBERR, std::string("Vishnu not available (Database error)")));
  mp.insert(std::pair<int, std::string>(ERRCODE_DBCONN, std::string("Vishnu not available (Database connection)")));
  mp.insert(std::pair<int, std::string>(ERRCODE_SYSTEM, std::string("Vishnu not available (System)")));
  mp.insert(std::pair<int, std::string>(ERRCODE_INVCOMP, std::string("Vishnu not available (Component)")));
  mp.insert(std::pair<int, std::string>(ERRCODE_INVMAPPER, std::string("Vishnu not available (Mapper)")));
}

std::string
SystemException::getMsg() const{
  if (mp.count(mval)){
    return mp[mval];
  }
  return "Internal Error: Unknown error code.\n";
}
