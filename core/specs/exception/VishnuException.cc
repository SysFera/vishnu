#include "VishnuException.hh"


VishnuException::VishnuException(){
  mmsgc = "";
  mtype = NONE;
  mval = -1;
  initMsg();
}

VishnuException::VishnuException(const VishnuException& e){
  mmsgc = e.getMsgComp();
  mtype = e.getTypeI();
  mval  = e.getMsgI();
  initMsg();
}

VishnuException::VishnuException(int msg, std::string msgComp){
  mval = msg;
  mmsgc = msgComp;
  mtype = NONE;
  initMsg();
}

void
VishnuException::appendMsgComp(std::string s){
  mmsgc += s;
}

int
VishnuException::getMsgI() const{
  return mval;
}
