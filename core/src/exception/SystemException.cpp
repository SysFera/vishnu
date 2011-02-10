#include "SystemException.hpp"


SystemException::SystemException():VishnuException(){initMsg();}

SystemException::SystemException(const SystemException& e):VishnuException(e){initMsg();}

SystemException::SystemException(int msg, std::string msgComp):VishnuException(msg, msgComp){initMsg();}

std::string
SystemException::getTypeS() const{
  switch (mtype){
  case 0 :
    return std::string("UMS");
    break;
  case 1 :
    return std::string("TMS");
    break;
  case 2 :
    return std::string("FMS");
    break;
  case 3 :
    return std::string("IMS");
    break;
  case 4 :
    return std::string("NONE");
    break;
  default :
    break;
  }
  return std::string("->not found<-");
}

void 
SystemException::initMsg(){
  mp.insert(std::pair<int, std::string>(1, std::string("Error in a DIET call")));
  mp.insert(std::pair<int, std::string>(2, std::string("Error with the database")));
  mp.insert(std::pair<int, std::string>(3, std::string("Error with a system call")));
  mp.insert(std::pair<int, std::string>(4, std::string("Error bad parameters")));
  mp.insert(std::pair<int, std::string>(5, std::string("Error invalid component")));
  mp.insert(std::pair<int, std::string>(6, std::string("Error invalid mapper")));
  mp.insert(std::pair<int, std::string>(7, std::string("Error it is a temporary password")));
}

std::string
SystemException::getMsg() const{
  if (mp.count(mval)){
    return mp[mval];
  }
  return "Unknown error code.\n";
}
