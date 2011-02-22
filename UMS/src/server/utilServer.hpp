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
#include <cstdio>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "VishnuException.hpp"
#include "SystemException.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;

/**
* \enum IdType
* \brief The type of id generated
*/
typedef enum IdType{
  MACHINE = 0,
  USER = 1,
  JOB = 2,
  FILETRANSFERT = 3
} IdType;

/**
* \class Format_t
* \brief The class used to get the variables of the string to parse
*/
class Format_t{
public :
  int start;
  int end;
  std::string value;
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

  static const std::string ROOTUSERNAME = "root";
  static const std::string DATABASENAME = "vishnu";

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
  buildExceptionString(VishnuException& e);
  /**
  * \brief Function to get a random number
  * \fn    int generate_numbers()
  * \return the number generated
  */
  int
  generate_numbers();

  int isMonth (const char * s);
  int isYear (const char * s);
  int isSite (const char * s);
  int isType (const char * s);
  int isUName (const char * s);
  int isMaName (const char * s);
  int isDay (const char * s);
  int isCpt (const char * s);

  void
  getKeywords (int* size, Format_t* array, const char* format, int cpt, IdType type,
	     std::string name, std::string site);
  std::string
  getGeneratedName (const char* format, int cpt, IdType type,
		  std::string name = "", std::string site ="");
  }
#endif//UTILSERVER