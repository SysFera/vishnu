/**
* \file utilServer.cpp
* \brief This file implements the utils functions of server objects
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011 
*/

#include "utilServer.hpp"

std::string Vishnuid::mvishnuid = "";
/**
* \brief Function to convert a string to int 
* \fn    std::string convertToString(const T& val);
* \param  val a value to convert to string
* \return int value of the corresponding string
*/
int utilServer::convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val); 
  str >> intValue;
  return static_cast<int> (intValue);
}
/**
* \brief Function to get the string associated to SystemException  
* \fn    buildExceptionString(SystemException& e)
* \param  e the exception caught
* \return int value of the corresponding string
*/
std::string
utilServer::buildExceptionString(SystemException& e) {
  std::string errorInfo;
  
  //To get the error code associated to the exception follows by #
  errorInfo = utilServer::convertToString(e.getMsgI())+"#";
  
  //To get exception information
  errorInfo.append(e.getMsg());
  errorInfo.append(" ");
  errorInfo.append(e.what());
  
  return errorInfo;
}
/**
* \brief Function to get a random number  
* \fn    int generate_numbers()
* \return the number generated
*/
int 
utilServer::generate_numbers() {
  
  boost::mt19937 gen;
  gen.seed(static_cast<boost::uint32_t>(std::time(0)));
  boost::uniform_int<> dist(1, 100000);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
  return die();
  
}
  