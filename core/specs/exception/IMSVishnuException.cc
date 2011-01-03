#include "IMSVishnuException.hh"

IMSVishnuException::IMSVishnuException(IMSVishnuException& e):VishnuException(e){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(int msg, std::string msgComp):VishnuException(msg, msgComp){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(){
  mtype = VishnuException::IMS;
}

std::string
IMSVishnuException::getMsg(){
  if (mval>=0 && mval<INBERR)
    return mp[mval];
  return "Unknown IMS error code.\n";
}
