#include "UMSVishnuException.hpp"

UMSVishnuException::UMSVishnuException(UMSVishnuException& e):SystemException(e){
  mtype = VishnuException::UMS;
  initMsg();
}
UMSVishnuException::UMSVishnuException(int msg, std::string msgComp):SystemException(msg, msgComp){
  mtype = VishnuException::UMS;
  initMsg();
}
UMSVishnuException::UMSVishnuException():SystemException(){
  mtype = VishnuException::UMS;
  initMsg();
}

void
UMSVishnuException::initMsg(){
  mp.insert(std::pair<int, std::string>(10, std::string("Error expired session")));
  // TODO add other error messages

}