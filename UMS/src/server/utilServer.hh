/**
* \file utilServer.hpp
* \brief This file presents the utils functions of server objects
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2001 
*/

#ifndef _UTILSERVER_H_
#define _UTILSERVER_H_

#include <iostream>
#include <sstream>

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
  * \fn    std::string convertToString(const T& val);
  * \param  val a value to convert to string
  * \return int value of the corresponding string
  */
  int convertToInt(std::string val);
  }
#endif//UTILSERVER