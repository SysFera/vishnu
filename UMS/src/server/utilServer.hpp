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
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "VishnuException.hpp"
#include "SystemException.hpp"
#include "POSTGREDatabase.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;
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
  static const std::string UMSMAPPERNAME = "UMS";

  /**
  * \brief Function to get a random number
  * \fn    int generate_numbers()
  * \return the number generated
  */
  int
  generate_numbers();

  /**
   * \brief Return true if the string starts with 'MONTH'
   * \fn int isMonth (const char * s)
   * \param s String to test
   * \return True if the string starts with 'MONTH'
   */
  int isMonth (const char * s);
  /**
   * \brief Return true if the string starts with 'YEAR'
   * \fn int isYear (const char * s)
   * \param s String to test
   * \return True if the string starts with 'YEAR'
   */
  int isYear (const char * s);
  /**
   * \brief Return true if the string starts with 'SITE'
   * \fn int isSite (const char * s)
   * \param s String to test
   * \return True if the string starts with 'SITE'
   */
  int isSite (const char * s);
  /**
   * \brief Return true if the string starts with 'TYPE'
   * \fn int isType (const char * s)
   * \param s String to test
   * \return True if the string starts with 'TYPE'
   */
  int isType (const char * s);
  /**
   * \brief Return true if the string starts with 'UNAME'
   * \fn int isUName (const char * s)
   * \param s String to test
   * \return True if the string starts with 'UNAME'
   */
  int isUName (const char * s);
  /**
   * \brief Return true if the string starts with 'MANAME'
   * \fn int isMaName (const char * s)
   * \param s String to test
   * \return True if the string starts with 'MANAME'
   */
  int isMaName (const char * s);
  /**
   * \brief Return true if the string starts with 'DAY'
   * \fn int isDay (const char * s)
   * \param s String to test
   * \return True if the string starts with 'DAY'
   */
  int isDay (const char * s);
  /**
   * \brief Return true if the string starts with 'CPT'
   * \fn int isCpt (const char * s)
   * \param s String to test
   * \return True if the string starts with 'CPT'
   */
  int isCpt (const char * s);
  /**
   * \brief Parse the format and fill the array with the variable and their value
   * \fn void getKeywords(int* size, Format_t* array,
   *                      const char* format,
   *                      int cpt, IdType type,
   *                      std::string name,
   *                      std::string site)
   * \param size Size of the array (OUT)
   * \param array Array to fill with variables and their value (OUT)
   * \param format The string to parse
   * \param cpt Value of the counter variable
   * \param type Type of the id to generate
   * \param name Name of the user or machine
   * \param site Site of the machine
   */
  void
  getKeywords (int* size, Format_t* array,
               const char* format,
               int cpt, IdType type,
	             std::string name,
               std::string site);
  /**
   * \brief Function to generate an ID
   * \fn std::string getGeneratedName (const char* format,
   *                                   int cpt,
   *                                   IdType type,
   *                                   std::string name = "",
   *                                   std::string site ="")
   * \param format Format to use to generate the id
   * \param cpt Value of the counter to use
   * \param type Type of the id generated
   * \param name Name of the user or machine (optionnal)
   * \param site Site of the machine (optionnal)
   */
  std::string
  getGeneratedName (const char* format,
                    int cpt,
                    IdType type,
                    std::string name = "",
                    std::string site ="");

  /**
  * \brief Function to get information from the table vishnu
  * \fn    string getAttrVishnu(string attrname, string vishnuid)
  * \param attrname the name of the attribut
  * \param vishnuid the id of the vishnu configuration
  * \return the corresponding attribut
  */
  std::string
  getAttrVishnu(std::string attrname, std::string vishnuid);

  /**
  * \brief Function to increment a counter of the table vishnu
  * \fn    incrementCpt(std::string cptName, int cpt)
  * \param cptName the name of the counter to increment
  * \param cpt     the current value of the counter
  * \return raises an exception
  */
  void
  incrementCpt(std::string cptName, int cpt);
}
#endif//UTILSERVER
