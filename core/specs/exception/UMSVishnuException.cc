#include "UMSVishnuException.hh"

UMSVishnuException::UMSVishnuException(UMSVishnuException& e):VishnuException(e){
  mtype = VishnuException::UMS;
}
UMSVishnuException::UMSVishnuException(int msg, std::string msgComp):VishnuException(msg, msgComp){
  mtype = VishnuException::UMS;
}
UMSVishnuException::UMSVishnuException(){
  mtype = VishnuException::UMS;
  mp.insert(std::pair<int, std::string>(1, std::string("Error in a DIET call")));
  mp.insert(std::pair<int, std::string>(2, std::string("Error with the database")));
  mp.insert(std::pair<int, std::string>(3, std::string("Error with a system call")));
  mp.insert(std::pair<int, std::string>(4, std::string("Error bad parameters")));
  mp.insert(std::pair<int, std::string>(5, std::string("Error invalid component")));
}

std::string
UMSVishnuException::getMsg(){
  if (mval>=0 && mval<UNBERR)
    return mp[mval];
  return "Unknown UMS error code.\n";
}
