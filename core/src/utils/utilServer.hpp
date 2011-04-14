/**
 * \file utilVishnu.cpp
 * \brief This file implements the utils functions of the vishnu system
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/02/2011
 */

#ifndef _UTILSERVER_H_
#define _UTILSERVER_H_

#include "ecore.hpp" // Ecore metamodel
#include "ecorecpp.hpp" // EMF4CPP utils
#include "UMS_Data.hpp"
//#include "TMS_Data.hpp"

/**
* \class Format_t
* \brief The class used to get the variables of the string to parse
*/
class Format_t{
public :
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
   * \brief Function to get information from the table vishnu
   * \param attrname the name of the attribut
   * \param vishnuid the id of the vishnu configuration
   * \return the corresponding attribut
   */
  std::string
  getAttrVishnu(std::string attrname, std::string vishnuid);

  /**
   * \brief Function to increment a counter of the table vishnu
   * \fn int  incrementCpt(std::string cptName, int cpt)
   * \param cptName the name of the counter to increment
   * \param cpt     the current value of the counter
   * \return raises an exception
   */
  void
  incrementCpt(std::string cptName, int cpt);

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
      UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
      ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

      //Parse the model
      ecorecpp::parser::parser parser;
      object_ptr = parser.load(objectSerialized)->as< T >();
    }
    catch (std::exception& e) {
      return false;
    }
    return true;
  }
}

#endif //_UTILSERVER_H_
