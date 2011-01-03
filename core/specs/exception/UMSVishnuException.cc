#include "UMSVishnuException.hh"

UMSVishnuException::UMSVishnuException(UMSVishnuException& e):VishnuException(e){
  mtype = VishnuException::UMS;
}
UMSVishnuException::UMSVishnuException(int msg, std::string msgComp):VishnuException(msg, msgComp){
  mtype = VishnuException::UMS;
}
UMSVishnuException::UMSVishnuException(){
  mtype = VishnuException::UMS;
}

std::string
UMSVishnuException::getMsg(){
  if (mval>=0 && mval<UNBERR)
    return mp[mval];
  return "Unknown UMS error code.\n";
}
