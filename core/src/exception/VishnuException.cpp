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

/**
* \brief Function to get the string associated to SystemException
* \fn    buildExceptionString()
* \return int value of the corresponding string
*/
std::string
VishnuException::buildExceptionString() {
  std::string errorInfo;

  //To get the error code associated to the exception follows by #
  errorInfo =  vishnu::convertToString(getMsgI())+"#";

  //To get exception information
  errorInfo.append(getMsg());
  errorInfo.append(" ");
  errorInfo.append(what());

  return errorInfo;
}