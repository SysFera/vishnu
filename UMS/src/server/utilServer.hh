/**
 * \file utilServer.hh
 * \brief This file presents the utils functions of server objects
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#ifndef _UTILSERVER_H_
#define _UTILSERVER_H_

#include <iostream>
#include <sstream>

/**
* \brief Function to encrypt data and password
* \fn    char* crypt(const char* clef, const char* salt);
* \param clef is the data to be encrypted
* \param salt This string is used to perturb the algorithm in one of 4096 different ways.
* \return the string encrypted
*/ 
char* crypt(const char* clef, const char* salt);
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


int convertToInt(std::string val);
/**
* \brief Function to check if an id is correct (higher than zero) 
* \fn    int checkId(int id);
* \param id is the id to be tested
* \return the id if correct or raises an exception
*/
int checkId(int id) ;

#endif//UTILSERVER