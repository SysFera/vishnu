/**
 * \file utilServer.hpp
 * \brief This file implements the utils functions of the vishnu system
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/02/2011
 */

#ifndef _UTILSERVER_H_
#define _UTILSERVER_H_

#include "ecore.hpp" // Ecore metamodel
#include "ecorecpp.hpp" // EMF4CPP utils
#include "UMS_Data.hpp"
#include "ExecConfiguration.hpp"

/**
* \class Format_t
* \brief The class used to get the variables of the string to parse
*/
struct Format_t{
  /**
   * \brief The  position of the first character of the variable
   */
  int start;
  /**
   * \brief The position of the last character of the variable
   */
  int end;

  /**
   * \brief The value of the variable
   */
  std::string value;
};

namespace vishnu {

  /**
   * \brief Position of the machine id in the machine table, starting at 0
   */
  static const int MIDPOS = 7;

  /**
   * \enum IdType
   * \brief The type of id generated
   */
  typedef enum{
    MACHINE = 0,/*!< For the machines*/
    USER = 1, /*!< For The users */
    JOB = 2, /*!< For the jobs */
    FILETRANSFERT = 3  /*!< For the file tranfers*/
  } IdType;
  /**
   * \enum CmdStatus
   * \brief The status of the command execution
   */
  typedef enum{
    CMDFAILED = 0, /*!< command failed */
    CMDSUCCESS = 1,/*!< command success */
  } CmdStatus;

  /**
  * \enum CmdType
  * \brief The type of the command
  */
  typedef enum{
    UMS = 0,/*!< For UMS command type */
    TMS = 1,/*!< For TMS command type */
    FMS = 2,/*!< For FMS command type */
    IMS = 3/*!< For IMS command type */
  } CmdType;

  /**
   * \brief If the process is in an undefined state
   */
  static const int PUNDEF = 0;
  /**
   * \brief If the process is running
   */
  static const int PRUNNING = 1;
  /**
   * \brief If the process is down
   */
  static const int PDOWN = 2;

  /**
   * \brief To register the server in the database
   * \param type: The type of the SeD
   * \param config: The configuration of the executable
   * \param cfg: The created path to the config file
   * \return 0 if the function succeeds or another value otherwise
   */
  int
  registerSeD(std::string type, ExecConfiguration config, std::string &cfg);
  /**
   * \brief To unregister the server in the database (set as stopped)
   * \param type: The type of the SeD
   * \param mid: The machine id
   * \return 0 if the function succeeds or another value otherwise
   */
  int
  unregisterSeD(std::string type, std::string mid);

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
   * \return 0 on success and -1 when an error occurs
   */
  int
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
   * \return the genarated ID
   */
  std::string
  getGeneratedName (const char* format,
                    int cpt,
                    IdType type,
                    std::string name = "",
                    std::string site ="");

  /**
   * \brief Function to get a specific vishnu counter
   * \param counterName the name of the counter
   * \param vishnuid the id of the vishnu configuration
   * \return The int counter value
   */
  int
  getVishnuCounter(std::string counterName, std::string vishnuId);

  /**
   * \brief Function to get information from the table vishnu
   * \param attrname the name of the attribut
   * \param vishnuid the id of the vishnu configuration
   * \param transacId the id of the transaction if one is used. -1 to ignore
   * \return the corresponding attribut
   */
  std::string
  getAttrVishnu(std::string attrname, std::string vishnuid, int transacId = -1);

  /**
   * \brief Function to increment a counter of the table vishnu
   * \fn int  incrementCpt(std::string cptName, int cpt)
   * \param cptName the name of the counter to increment
   * \param cpt     the current value of the counter
   * \param transacId the id of the transaction if one is used. -1 to ignore
   * \return raises an exception
   */
  void
  incrementCpt(std::string cptName, int cpt, int transacId = -1);

  /**
  * \brief Function to get an Id generated by VISHNU
  * \param vishnuId the vishnu Id
  * \param counterName the name of the counter used to generate the Id
  * \param formatName the name of the format
  * \param type the type of the Id generated
  * \param stringforgeneration the string used for generation
  * \return the corresponding conversion
  */
  std::string
  getObjectId(int vishnuId,
              std::string counterName,
              std::string formatName,
              IdType type,
              std::string stringforgeneration);

  /**
   * \brief Function to parse the EMF object
   * \param objectSerialized the EMF object serialized
   * \param object_ptr the object build with the objectSerialized
   * \param msgComp an exception message
   * \return  true if success else false
   */
  template<class T>
  bool parseEmfObject(const std::string& objectSerialized, T*& object_ptr, const std::string msgComp=std::string()) {

    object_ptr = NULL;
    try {
      //CREATE DATA MODEL
      T tmpObject;
      ecore::EPackage_ptr ecorePackage = tmpObject._eClass()->getEPackage();
      ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

      //Parse the model
      ecorecpp::parser::parser parser;
      object_ptr = parser.load_str(objectSerialized)->as< T >();
    }
    catch (std::exception& e) {
      return false;
    }
    return true;
  }

/**
 * \brief Function to parse a system error message
 * \param errorMsg the error message
 * \return the parsed message
 */
std::string parseErrorMessage (const std::string& errorMsg);

}
#endif //_UTILSERVER_H_
