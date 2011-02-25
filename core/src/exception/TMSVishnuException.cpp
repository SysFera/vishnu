#include "TMSVishnuException.hpp"


TMSVishnuException::TMSVishnuException(TMSVishnuException& e):UserException(e){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException():UserException(){
  mtype = VishnuException::TMS;
}
