#include "IMSVishnuException.hpp"


IMSVishnuException::IMSVishnuException(const IMSVishnuException& e):UserException(e){
  mtype = VishnuException::IMS;
  initMsg();
}

IMSVishnuException::IMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::IMS;
  initMsg();
}

IMSVishnuException::IMSVishnuException():UserException(){
  mtype = VishnuException::IMS;
  initMsg();
}

void
IMSVishnuException::initMsg(){

}
