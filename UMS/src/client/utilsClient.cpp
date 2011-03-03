/**
 * \file utilsClient.hpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "utilsClient.hpp"

/**
 * \brief Function to spread error message to C++ API, Python API, Web service API and command line program
 * \fn void sendErrorMsg(const std::string& msg) 
 * \param msg to spread 
 */
void sendErrorMsg(const std::string& msg) {
  std::string msgWithHead = "Error: "+msg;
  UMSVishnuException e(1, msgWithHead);
  throw e;
}

/**
 * \brief Function to split the receiving message into error code and message error 
 * \fn void checkErrorMsg(const std::string& msg) 
 * \param msg to split 
 */
void checkErrorMsg(const std::string& msg) {

  if(msg.size() > 0 ) {
    size_t pos = msg.find('#');
    if(pos!=std::string::npos) {
       std::string codeInString = msg.substr(0,pos);
       if(codeInString.size()!=0) {
         std::istringstream isCode(codeInString);
         int ret;
         isCode >> ret;
         std::string message = msg.substr(pos+1);
         throw UMSVishnuException(ret, message);
      }
    } else {
          throw UMSVishnuException(1, msg);
    }
  }

}

void checkIfTextIsEmpty(const std::string& text, const std::string& compMsg, const int& execpetionType) {

   if(text.size()==0) {
      throw UMSVishnuException(execpetionType, compMsg);
   }

} 
