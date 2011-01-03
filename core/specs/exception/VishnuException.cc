#include "VishnuException.hh"


VishnuException::VishnuException(){
  mmsgc = "";
  mtype = NONE;
  mval = -1;
}

VishnuException::VishnuException(VishnuException& e){
  mmsgc = e.getMsgComp();
  mtype = e.getTypeI();
  mval  = e.getMsgI();
}

VishnuException::VishnuException(int msg, std::string msgComp){
  mval = msg;
  mmsgc = msgComp;
  mtype = NONE;
}

std::string
VishnuException::getTypeS(){
  switch (mtype){
  case 0 :
    return std::string("UMS");
    break;
  case 1 :
    return std::string("TMS");
    break;
  case 2 :
    return std::string("FMS");
    break;
  case 3 :
    return std::string("IMS");
    break;
  case 4 :
    return std::string("NONE");
    break;
  }
  return std::string("->not found<-");
}

void
VishnuException::appendMsgComp(std::string s){
  mmsgc += s;
}

int
VishnuException::getMsgI(){
  return mval;
}
