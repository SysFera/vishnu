/**
 * \file utilsClient.hpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "utilsClient.hpp"

/**
 * \brief Function to spread error message to C++ API, Python API, Web service API and command line program
 * \fn void raiseDietMsgException(const std::string& msg)
 * \param msg to spread
 * \return raises an exception on error
 */
void raiseDietMsgException(const std::string& msg) {
  SystemException e(ERRCODE_DIET, msg);
  throw e;
}

/**
 * \brief Function to split the receiving message into error code and message error
 * \fn void raiseExceptionIfNotEmptyMsg(const std::string& msg)
 * \param msg to split
 * \return raises an exception on error
 */
void raiseExceptionIfNotEmptyMsg(const std::string& msg) {

  if(msg.size() > 0 ) {
    size_t pos = msg.find('#');
    if(pos!=std::string::npos) {
      std::string codeInString = msg.substr(0,pos);
      if(codeInString.size()!=0) {
        std::istringstream isCode(codeInString);
        int code;
        isCode >> code;
        std::string message = msg.substr(pos+1);
        if (code<10){
          throw SystemException(code, message);
        } else if (code<100){
          throw UMSVishnuException(code, message);
        } else {
          std::string tmp = "Invalid code: ";
          tmp += vishnu::convertToString(code);
          tmp += " with message: ";
          tmp += message;
          throw SystemException(ERRCODE_INVEXCEP, tmp);
        }
      }
    } else {
      throw UMSVishnuException(-1, msg);
    }
  }

}

/**
 * \brief Function to check if a text is empty
 * \param text The text to check
 * \param comMsg The complementary message to print
 * \param execpetionType The type of exception to raise
 * \return raises an exception on error
 */
void checkIfTextIsEmpty(const std::string& text, const std::string& compMsg, const int& execpetionType) {

  if(text.size()==0) {
    throw UMSVishnuException(execpetionType, compMsg);
  }

}

/**
 * \brief Function to check if an email is valid
 * \param mail The email to check
 * \return raises an exception on error
 */
void checkEmail(const std::string& mail) {

  if(mail.find("@")==std::string::npos) {
    throw UMSVishnuException(ERRCODE_INVALID_MAIL_ADRESS);
  }

}
