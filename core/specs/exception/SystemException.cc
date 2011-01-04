#include "SystemException.hh"


SystemException::SystemException(){
  mmsgc = "";
  mtype = VishnuException::NONE;
  mval = -1;
  initMsg();
}

SystemException::SystemException(const SystemException& e){
  mmsgc = e.getMsgComp();
  mtype = e.getTypeI();
  mval  = e.getMsgI();
  initMsg();
}

SystemException::SystemException(int msg, std::string msgComp){
  mval = msg;
  mmsgc = msgComp;
  mtype = VishnuException::NONE;
  initMsg();
}

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
}

std::string
SystemException::getMsg() const{
//  std::map<int, std::string>::const_iterator it = mp.find(mval);
  //if (it != mp.end())
   // return it->second;
      if (mval>=1 && mval<=static_cast<int>(mp.size()))
      return mp[mval];
  return "Unknown error code.\n";
}
