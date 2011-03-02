/**
 * \file ServerUMS.hpp
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef _SERVERUMS_H_
#define _SERVERUMS_H_

#include <string>
/**
 * \brief Number of service in UMS
 */
#define NB_SRV 25
#include "DIET_server.h"

static const char* SRV[NB_SRV] = {
  "sessionConnect",
  "sessionReconnect",
  "sessionClose",
  "userCreate",
  "userUpdate",
  "userDelete",
  "userPasswordChange",
  "userPasswordReset",
  "machineCreate",
  "machineUpdate",
  "machineDelete",
  "localAccountCreate",
  "localAccountUpdate",
  "localAccountDelete",
  "configurationSave",
  "configurationRestore",
  "optionValueSet",
  "optionValueSetDefault",
  "sessionList",
  "localAccountList",
  "machineList",
  "commandList",
  "optionValueList",
  "userList",
  "restore"};
/**
 * \class ServerUMS
 * \brief This class describes the UMS server
 */
class ServerUMS {
public :

  /**
   * \brief To get the unique instance of the server
   */
  static ServerUMS*  getInstance();

  /**
  * \brief To get the vishnuId
  * \fn int getVishnuId()
  * \return the path of the configuration file
  */
  int
  getVishnuId();

  /**
  * \brief To get the path to the sendmail script
  * \fn std::string getSendmailScriptPath()
  * \return the path of the configuration file
  */
  std::string
  getSendmailScriptPath();

   /**
   * \brief To initialize the UMS server
   * \fn int init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword, std::string sendmailScriptPath)
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbType   The type of the database (POSTGREDB|ORACLEDB)
   * \param dbHost   The host of the database server
   * \param dbUsername The name of the database user on the server
   * \param dbPassword The password of the database user on the server
   * \param sendmailScriptPath The path to the script for sending emails
   * \return an error code (0 if success and 1 if an error occurs)
   */
  int
  init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword,
       std::string sendmailScriptPath);

  /**
   * \brief Destructor, raises an exception on error
   * \fn ~ServerUMS()
   */
  ~ServerUMS();

private :

  /**
   * \brief Constructor, private because class is singleton
   */
  ServerUMS();

  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The singleton reference
   */
  static ServerUMS*  minstance;
  /**
  * \brief The path to the sendmail script
  */
  std::string msendmailScriptPath;
  /**
  * \brief The vishnu id
  */
  int mvishnuId;
  /**
  * \brief Structure representing a profile description
  */
  diet_profile_desc_t* mprofile;
};
#endif // SERVERUMS
