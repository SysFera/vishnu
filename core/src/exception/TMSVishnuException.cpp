#include "TMSVishnuException.hpp"


TMSVishnuException::TMSVishnuException(TMSVishnuException& e):SystemException(e){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException(int msg, std::string msgComp):SystemException(msg, msgComp){
  mtype = VishnuException::TMS;
}

TMSVishnuException::TMSVishnuException():SystemException(){
  mtype = VishnuException::TMS;
}
