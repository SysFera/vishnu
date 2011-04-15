#include "TMSVishnuException.hh"


TMSVishnuException::TMSVishnuException(const TMSVishnuException& e):UserException(e){
  mtype = VishnuException::TMS;
  initMsg();
}

TMSVishnuException::TMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::TMS;
  initMsg();
}

TMSVishnuException::TMSVishnuException():UserException(){
  mtype = VishnuException::TMS;
  initMsg();
}

void
TMSVishnuException::initMsg(){
}
