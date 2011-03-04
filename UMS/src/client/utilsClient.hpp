/**
 * \file utilsClient.hpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#ifndef  _utilsClient_H_  
#define  _utilsClient_H_

#include <string>
#include <iostream>
#include <sstream>

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
 * \fn inline void printSuccessMessage(const std::string& errorInfo) 
 */
inline void printSuccessMessage() {
  std::cout << "The service was performed successfully" << std::endl;
}

/**
 * \brief Function to spread error message to C++ API, Python API, Web service API and command line program
 * \fn void raiseDietMsgException(const std::string& msg) 
 * \param msg to spread 
 */
void raiseDietMsgException(const std::string& msg);

/**
 * \brief Function to split the receiving message into error code and message error 
 * \fn void raiseExceptionIfNotEmptyMsg(const std::string& msg) 
 * \param msg to split 
 */
void raiseExceptionIfNotEmptyMsg(const std::string& msg);

/**
 * \brief Function to check if a text is empty 
 * \param text The text to check
 * \param comMsg The complementary message to print
 * \param execpetionType The type of exception to raise
 * \return raises an exception on error 
 */
void checkIfTextIsEmpty(const std::string& text, const std::string& compMsg, const int& execpetionType);

/**
 * \brief Function to check if an email is valid  
 * \param mail The email to check
 * \return raises an exception on error 
 */
void checkEmail(const std::string& mail);
#endif
