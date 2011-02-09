#include "FMSVishnuException.hpp"

FMSVishnuException::FMSVishnuException(FMSVishnuException& e):SystemException(e){
  mtype = VishnuException::FMS;
}

FMSVishnuException::FMSVishnuException(int msg, std::string msgComp):SystemException(msg, msgComp){
  mtype = VishnuException::FMS;
}

FMSVishnuException::FMSVishnuException(){
  mtype = VishnuException::FMS;
}

std::string
FMSVishnuException::getMsg(){
  if (mval>=0 && mval<FNBERR)
    return mp[mval];
  return "Unknown FMS error code.\n";
}
