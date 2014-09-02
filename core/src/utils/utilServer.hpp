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
#include "Database.hpp"

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
  std::string machine_address;
  /**
   * @brief The num machine identifier
   */
  std::string num_machine;
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
    FMS = 2 /*!< For FMS command type */
  } CmdType;

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

  template<class T>
  std::string emfSerializer(T* objectPtr) {
    ::ecorecpp::serializer::serializer _ser;
    return _ser.serialize_str(const_cast<T*>(objectPtr));
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
  createDir(const std::string& path, bool isWorkingdir=false);
  /**
   * \brief Show the version of vishnu
   * \param server The server to display the version
   * \return EXIT_SUCCESS
   */
  int
  showVersion(const std::string& server); // FIXME: remove me
  /**
   * @brief Displau the version of Vishnu
   * @return EXIT_SUCCESS
   */
  int
  showVersion();
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

  /**
   * @brief Get the template to build object identifier
   * @param objectType The object type
   * @return The template string
   */
  std::string
  getIdFormatTemplate(IdType objectType);

}
#endif //_UTILSERVER_H_
