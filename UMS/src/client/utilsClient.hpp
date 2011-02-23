/**
 * \file utilsClient.hpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes 
 * \authors Daouda Traore (daouda.traore@sysfera.com)
 */
#ifndef  _utilsClient_H_  
#define  _utilsClient_H_

#include <string>
#include <iostream>
#include <cstring>
#include <iostream>
#include <sstream>
//#include <algorithm>

#include "UMSVishnuException.hpp"

/**
 * \brief Function to print an error message and the line and file where this error has been occured 
 * \fn  inline void errMsg(const std::string& msg) 
 * \param msg to print 
 */
inline void errMsg(const std::string& msg) {
  std::cerr << "Error line " << __LINE__-2  << " in file " << __FILE__ << " with message: ";
  std::cerr << std::endl;
  std::cerr << msg << std::endl;
}

/**
 * \brief Function to print success message 
 * \fn inline void printSuccessMessage(char* errorInfo) 
 * \param errorInfo to check 
 */
inline void printSuccessMessage(char* errorInfo) {
  if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
}

/**
 * \brief Function to spread error message to C++ API, Python API, Web service API and command line program
 * \fn void sendErrorMsg(const std::string& msg) 
 * \param msg to spread 
 */
void sendErrorMsg(const std::string& msg);

/**
 * \brief Function to split the receiving message into error code and message error 
 * \fn void checkErrorMsg(const std::string& msg) 
 * \param msg to split 
 */
void checkErrorMsg(const std::string& msg);


#endif
