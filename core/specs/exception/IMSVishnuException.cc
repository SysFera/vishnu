#include "IMSVishnuException.hh"

IMSVishnuException::IMSVishnuException(IMSVishnuException& e):SystemException(e){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(int msg, std::string msgComp):SystemException(msg, msgComp){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(){
  mtype = VishnuException::IMS;
}
