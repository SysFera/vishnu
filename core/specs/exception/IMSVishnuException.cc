#include "IMSVishnuException.hh"

IMSVishnuException::IMSVishnuException(const IMSVishnuException& e):SystemException(e){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(int msg, std::string msgComp):SystemException(msg, msgComp){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException():SystemException(){
  mtype = VishnuException::IMS;
}
