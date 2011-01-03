#include "UMSVishnuException.hh"

UMSVishnuException::UMSVishnuException(UMSVishnuException& e):SystemException(e){
  mtype = VishnuException::UMS;
}
UMSVishnuException::UMSVishnuException(int msg, std::string msgComp):SystemException(msg, msgComp){
  mtype = VishnuException::UMS;
}
UMSVishnuException::UMSVishnuException(){
  mtype = VishnuException::UMS;
}
