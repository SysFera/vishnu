#include "FMSVishnuException.hpp"

FMSVishnuException::FMSVishnuException(FMSVishnuException& e):UserException(e){
  mtype = VishnuException::FMS;
}

FMSVishnuException::FMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::FMS;
}

FMSVishnuException::FMSVishnuException():UserException(){
  mtype = VishnuException::FMS;
}

std::string
FMSVishnuException::getMsg(){
  if (mval>=0 && mval<FNBERR)
    return mp[mval];
  return "Unknown FMS error code.\n";
}
