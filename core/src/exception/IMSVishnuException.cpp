#include "IMSVishnuException.hpp"

IMSVishnuException::IMSVishnuException(const IMSVishnuException& e):UserException(e){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::IMS;
}

IMSVishnuException::IMSVishnuException():UserException(){
  mtype = VishnuException::IMS;
}
