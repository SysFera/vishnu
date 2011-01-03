#include "IMSVishnuException.hh"

IMSVishnuException::IMSVishnuException(IMSVishnuException& e):VishnuException(e){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(int msg, std::string msgComp):VishnuException(msg, msgComp){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(){
  mtype = VishnuException::IMS;
  mp.insert(std::pair<int, std::string>(1, std::string("Error in a DIET call")));
  mp.insert(std::pair<int, std::string>(2, std::string("Error with the database")));
  mp.insert(std::pair<int, std::string>(3, std::string("Error with a system call")));
  mp.insert(std::pair<int, std::string>(4, std::string("Error bad parameters")));
  mp.insert(std::pair<int, std::string>(5, std::string("Error invalid component")));
}

std::string
IMSVishnuException::getMsg(){
  if (mval>=0 && mval<INBERR)
    return mp[mval];
  return "Unknown IMS error code.\n";
}
