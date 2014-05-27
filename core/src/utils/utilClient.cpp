/**
 * \file utilClient.cpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */
#include "utilClient.hpp"
#include <iostream>                     // for basic_istream::operator>>, etc
#include "FMSVishnuException.hpp"       // for FMSVishnuException
#include "TMSVishnuException.hpp"       // for TMSVishnuException
#include "UMSVishnuException.hpp"       // for UMSVishnuException
#include "utilVishnu.hpp"

/**
 * \brief Function to spread error message to C++ API, Python API, Web service API and command line program
 * \param msg to spread
 * \return raises an exception on error
 */
void raiseCommunicationMsgException(const std::string& msg) {
  throw SystemException (ERRCODE_COMMUNICATION, msg);
}

/**
 * \brief Function to split the receiving message into error code and message error
 * \param msg to split
 * \return raises an exception on error
 */
void raiseExceptionIfNotEmptyMsg(const std::string& msg) {

  if (! msg.empty()) {
    size_t pos = msg.find('#');
    if(pos!=std::string::npos) {
      std::string codeInString = msg.substr(0, pos);
      if(codeInString.size() != 0) {
        std::istringstream isCode(codeInString);
        int code;
        isCode >> code;
        std::string message = msg.substr(pos + 1);
        if (code < 10){
          throw SystemException(code, message);
        } else if (code < 100){
          throw UMSVishnuException(code, message);
        } else if ((code >= 100) && (code < 200)) {
          throw TMSVishnuException(code, message);
        }else if ((code >= 200) && (code < 300)){
          throw FMSVishnuException(code, message);
        } else {
          std::string tmp = "Invalid code: ";
          tmp += vishnu::convertToString(code);
          tmp += " with message: ";
          tmp += message;
          throw SystemException(ERRCODE_INVEXCEP, tmp);
        }
      }
    } else {
      throw SystemException(ERRCODE_INVEXCEP, msg);
    }
  }
}
