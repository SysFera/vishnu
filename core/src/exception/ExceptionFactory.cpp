#include "ExceptionFactory.hpp"
#include "SystemException.hpp"
#include "UMSVishnuException.hpp"

using namespace std;
using namespace vishnu;

ExceptionFactory::ExceptionFactory(){
}

ExceptionFactory::~ExceptionFactory(){
}

VishnuException*
ExceptionFactory::create(int code, string msg){
  if (code<10){
    return new SystemException(code, msg);
  }else if (code<100){
    return new UMSVishnuException(code, msg);
  }
  string tmp = "Invalid code: ";
  tmp += convertToString(code);
  tmp += " with message: ";
  tmp += msg;
  return new SystemException(ERRCODE_INVEXCEP, tmp);
}
