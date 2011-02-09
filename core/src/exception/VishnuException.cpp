#include "VishnuException.hpp"


VishnuException::VishnuException(){
  mmsgc = "";
  mtype =VishnuException::NONE;
  mval = -1;
}

VishnuException::VishnuException(const VishnuException& e){
  mmsgc = e.what();
  mtype = e.getTypeI();
  mval  = e.getMsgI();
}

VishnuException::VishnuException(int msg, std::string msgComp){
  mval = msg;
  mmsgc = msgComp;
  mtype = VishnuException::NONE;
}

void
VishnuException::appendMsgComp(std::string s){
  mmsgc += s;
}

int
VishnuException::getMsgI() const{
  return mval;
}
