#include "TMSVishnuException.hh"


TMSVishnuException::TMSVishnuException(TMSVishnuException& e):VishnuException(e){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException(int msg, std::string msgComp):VishnuException(msg, msgComp){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException(){
  mtype = VishnuException::TMS;
}

std::string
TMSVishnuException::getMsg(){
  if (mval>=0 && mval<TNBERR)
    return mp[mval];
  return "Unknown TMS error code.\n";
}
