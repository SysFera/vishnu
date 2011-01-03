#include "TMSVishnuException.hh"


TMSVishnuException::TMSVishnuException(TMSVishnuException& e):VishnuException(e){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException(int msg, std::string msgComp):VishnuException(msg, msgComp){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException(){
  mtype = VishnuException::TMS;
  mp.insert(std::pair<int, std::string>(1, std::string("Error in a DIET call")));
  mp.insert(std::pair<int, std::string>(2, std::string("Error with the database")));
  mp.insert(std::pair<int, std::string>(3, std::string("Error with a system call")));
  mp.insert(std::pair<int, std::string>(4, std::string("Error bad parameters")));
  mp.insert(std::pair<int, std::string>(5, std::string("Error invalid component")));
}

std::string
TMSVishnuException::getMsg(){
  if (mval>=0 && mval<TNBERR)
    return mp[mval];
  return "Unknown TMS error code.\n";
}
