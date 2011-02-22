/**
 * \file utilsClient.hpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes 
 * \authors Daouda Traore (daouda.traore@sysfera.com)
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
    char* codeInString = strtok(strdup(msg.c_str()), "#");
  if(codeInString!=NULL) {
    std::istringstream isCode(codeInString);
    int ret;
    isCode >> ret;
    char* message = strtok(NULL, codeInString);
    if(message!=NULL) {
      std::cout << "ERROR:" << message << std::endl;
      UMSVishnuException e(ret, message);
      throw e;
      }
    }
 }
}

