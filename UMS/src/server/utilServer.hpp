/**
* \file utilServer.hpp
* \brief This file presents the utils functions of server objects
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011 
*/

#ifndef _UTILSERVER_H_
#define _UTILSERVER_H_

#include <iostream>
#include <sstream>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include "SystemException.hpp"

/**
* \class Vishnuid
* \brief The Vishnuid class encapsulates the global vishnu id
*/
class Vishnuid {
  public:
    static std::string mvishnuid;
};
/**
* \brief Function to encrypt data and password
* \fn    char* crypt(const char* clef, const char* salt);
* \param clef is the data to be encrypted
* \param salt This string is used to perturb the algorithm
* \return the string encrypted
*/ 
char* crypt(const char* clef, const char* salt);

/**
* \namespace utilServer
* \brief The utils functions of server objects
*/  
namespace utilServer {
  /**
  * \brief The id of the VISHNU configuration registered in the database
  */
  static std::string mvishnuid = "";  
  /**
  * \brief Generic function to convert an object to string 
  * \fn    std::string convertToString(const T& val);
  * \param T is a generic data to be transformed to string
  * \return the string version of T
  */
  template <class T>
  std::string convertToString(T val) {
    std::ostringstream out;
    out << val;
    return out.str();
  }
  /**
  * \brief Function to convert a string to int 
  * \fn    std::string convertToString(const T& val)
  * \param  val a value to convert to string
  * \return int value of the corresponding string
  */
  int convertToInt(std::string val);
  /**
  * \brief Function to get the string associated to SystemException  
  * \fn    std::string buildExceptionString(SystemException& e)
  * \param  e the exception caught
  * \return int value of the corresponding string
  */
  std::string
  buildExceptionString(SystemException& e);
  /**
  * \brief Function to get a random number  
  * \fn    int generate_numbers()
  * \return the number generated
  */
  int 
  generate_numbers();
  }
   
#endif//UTILSERVER