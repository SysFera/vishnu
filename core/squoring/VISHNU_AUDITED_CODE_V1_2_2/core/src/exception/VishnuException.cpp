#include "VishnuException.hpp"
#include <sstream>

template <class T>
std::string convertToString(T val) {
    std::ostringstream out;
    out << val;
    return out.str();
}

VishnuException::VishnuException(){
  mmsgc = "";
  mtype =VishnuException::NONE;
  mval = -1;
}

VishnuException::VishnuException(const VishnuException& e){
  mmsgc = e.getMsgComp();
  mtype = e.getTypeI();
  mval  = e.getMsgI();
}

VishnuException::VishnuException(int msg, std::string msgComp){
  mval = msg;
  mmsgc = msgComp;
  mtype = VishnuException::NONE;
}

const char*
VishnuException::what() const throw()
{
  mfullMsg = "VISHNU Error " + convertToString(getMsgI());
  mfullMsg += " : ";
  mfullMsg += getMsg();
  mfullMsg += " [";
  mfullMsg += mmsgc;
  mfullMsg += " ]";
  return mfullMsg.c_str();
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
* \return a serialized format of the exception
*/
std::string
VishnuException::buildExceptionString() const {
  std::string errorInfo;

  //To get the error code associated to the exception follows by #
  errorInfo =  convertToString(getMsgI())+"#";

  //To get exception complementary message
  errorInfo.append(mmsgc);

  return errorInfo;
}
