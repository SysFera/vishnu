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
#include "Database.hpp"


#define LOG(msg, logLevel) if (logLevel) std::clog << msg <<"\n"

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


struct UserSessionInfo {
  /**
   * @brief The session number
   */
  int num_session;

  /**
   * @brief The user number
   */
  int num_user;

  /**
   * @brief The user privilege
   */
  int user_privilege;

  /**
   * @brief userid
   */
  std::string userid;

  /**
   * @brief The user local account
   */
  std::string user_aclogin;

  /**
   * @brief The user local account home
   */
  std::string user_achome;

  /**
   * @brief The target machine name
   */
  std::string machine_name;
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
    FILETRANSFERT = 3,  /*!< For the file tranfers*/
    AUTH = 4,  /*!< For the authentication system*/
    WORK = 5  /*!< For the work*/
  } IdType;
  /**
   * \enum CmdStatus
   * \brief The status of the command execution
   */
  typedef enum{
    CMDFAILED = 0, /*!< command failed */
    CMDSUCCESS = 1 /*!< command success */
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
   * \brief Return if the server on the machine is already declared
   * \param urlsup the url of the supervisor
   * \param mid the machine id
   * \param type the type of server
   * \return if the server is already existant in the database on this machine
   */
  bool
  isNew(std::string urlsup, std::string mid, std::string type);

  /**
   * \brief Return true if the string starts with 'MONTH'
   * \param s String to test
   * \return True if the string starts with 'MONTH'
   */
  bool
  isMonth (const char * s);
  /**
   * \brief Return true if the string starts with 'YEAR'
   * \param s String to test
   * \return True if the string starts with 'YEAR'
   */
  bool
  isYear (const char * s);
  /**
   * \brief Return true if the string starts with 'SITE'
   * \param s String to test
   * \return True if the string starts with 'SITE'
   */
  bool
  isSite (const char * s);
  /**
   * \brief Return true if the string starts with 'TYPE'
   * \param s String to test
   * \return True if the string starts with 'TYPE'
   */
  bool
  isType (const char * s);
  /**
   * \brief Return true if the string starts with 'UNAME'
   * \param s String to test
   * \return True if the string starts with 'UNAME'
   */
  bool
  isUName (const char * s);
  /**
   * \brief Return true if the string starts with 'NAME'
   * \param s String to test
   * \return True if the string starts with 'NAME'
   */
  bool
  isName (const char * s);
  /**
   * \brief Return true if the string starts with 'MANAME'
   * \param s String to test
   * \return True if the string starts with 'MANAME'
   */
  bool
  isMaName (const char * s);
  /**
   * \brief Return true if the string starts with 'DAY'
   * \param s String to test
   * \return True if the string starts with 'DAY'
   */
  bool
  isDay (const char * s);
  /**
   * \brief Return true if the string starts with 'CPT'
   * \param s String to test
   * \return True if the string starts with 'CPT'
   */
  bool
  isCpt (const char * s);

  /**
   * \brief Parse the format and fill the array with the variable and their value
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
   * \brief Function to check if the objectid already exist in column idname in the table
   * \param table the name of the table
   * \param idname the name of the column
   * \param objectId the value to test if it already exists
   *\return True if the objectId is new, false if it already exists
   */
  bool
  checkObjectId(std::string table,
                std::string idname,
                std::string objectId);


  /**
   * \brief Function to get a specific vishnu counter
   * \param vishnuId the id of the vishnu configuration
   * \param type : the type of id generated
   * \return The int counter value
   */
  int
  getVishnuCounter(std::string vishnuId, IdType type);

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
   * \brief Function to increment a counter of the table vishnu.
   * \param cptName the name of the counter to increment
   * \param cpt     the current value of the counter
   * \param transacId the id of the transaction if one is used. -1 to ignore
   */
  void
  incrementCpt(std::string cptName, int cpt, int transacId = -1);

  /**
   * \brief To set the objectId in the specified row in the database
   * \param key : the key to identify the reserved row
   * \param objectId : the objectId to set
   * \param type : the type of the object
   */
  void
  reserveObjectId(int key, std::string &objectId, IdType type);

  /**
  * \brief Function to get an Id generated by VISHNU
  * \param vishnuId the vishnu Id
  * \param formatName the name of the format
  * \param type the type of the Id generated
  * \param stringforgeneration the string used for generation
  * \return the corresponding conversion
  */
  std::string
  getObjectId(int vishnuId,
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
  bool parseEmfObject(const std::string& objectSerialized,
                      T*& object_ptr,
                      const std::string& msgComp = std::string()) {
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
  std::string
  parseErrorMessage (const std::string& errorMsg);

  /**
   * \brief Return the status value from file
   * \param file The status file
   * \return the int value
   */
  int
  getStatusValue (const std::string& file);

  /**
   * \brief Function to move the file parameters to a given directory
   * \param fileparams a list of key=value
   * \param dir the directory where we create
   * \return the new directory containing the file
   */
  std::string
  moveFileData(const std::string& fileparams, std::string dir);

  /**
   *  \brief Function to create a working directory. The working directory needs rwxt permissions
   *  \param path the path of the working directory
   *  \param isWorkingdir tell whether the directory will be a working directory
   */
  void
  createDir(const std::string& path, const bool& isWorkingdir=false);
  /**
   * \brief Show the version of vishnu
   * \param server The server to display the version
   * \return EXIT_SUCCESS
   */
  int
  showVersion(std::string server);

  /**
   * @brief Validate session key and return details on the user and the session
   * @param authKey The authentication key
   * @param machineId The machine Id
   * @param databasePtr A pointer to a database instance
   * @param info The resulting information
   */
  void
  validateAuthKey(const std::string& authKey,
                  const std::string& machineId,
                  Database* databasePtr,
                  UserSessionInfo& info);


  /**
   * @brief Validate session key and return details on the user and the session
   * @param authKey The authentication key
   * @param databasePtr A pointer to a database instance
   * @param info The resulting information
   */
  void
  validateAuthKey(const std::string& authKey,
                  Database* databasePtr,
                  UserSessionInfo& info);
}
#endif //_UTILSERVER_H_
