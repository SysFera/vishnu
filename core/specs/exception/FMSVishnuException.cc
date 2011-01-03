#include "FMSVishnuException.hh"

FMSVishnuException::FMSVishnuException(FMSVishnuException& e):VishnuException(e){
  mtype = VishnuException::FMS;
}

FMSVishnuException::FMSVishnuException(int msg, std::string msgComp):VishnuException(msg, msgComp){
  mtype = VishnuException::FMS;
}

FMSVishnuException::FMSVishnuException(){
  mtype = VishnuException::FMS;
  mp.insert(std::pair<int, std::string>(1, std::string("Error in a DIET call")));
  mp.insert(std::pair<int, std::string>(2, std::string("Error with the database")));
  mp.insert(std::pair<int, std::string>(3, std::string("Error with a system call")));
  mp.insert(std::pair<int, std::string>(4, std::string("Error bad parameters")));
  mp.insert(std::pair<int, std::string>(5, std::string("Error invalid component")));
}

std::string
FMSVishnuException::getMsg(){
  if (mval>=0 && mval<FNBERR)
    return mp[mval];
  return "Unknown FMS error code.\n";
}
