/**
 * \file ServerUMS.hpp
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef _SERVERUMS_H_
#define _SERVERUMS_H_

#include <string>
#include "internalApi.hpp"
#define NB_SRV 25

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
   * \brief To get the path of the configuration file used by the UMS server
   * \fn std::string getCfg()
   * \return the path of the configuration file
   */
  std::string
  getCfg();

   /**
   * \brief To initialize the UMS server with individual parameters instead of configuration file
   * \fn int init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword)
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbType   The type of the database (POSTGREDB|ORACLEDB)
   * \param dbHost   The host of the database server
   * \param dbUsername The name of the database user on the server
   * \param dbPassword The password of the database user on the server
   * \return an error code (0 if success and 1 if an error occurs)
   */
  int
  init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword);

  /**
   * \brief Constructor, raises an exception on error
   * \fn ServerUMS(std::string cfg)
   * \param cfg The vishnu configuration filepath
   */
  ServerUMS(std::string cfg="");

  /**
   * \brief Destructor, raises an exception on error
   * \fn ~ServerUMS()
   */
  ~ServerUMS();

private :

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The VISHNU configuration file path
  */
  std::string mvishnucfg;
  /**
  * \brief Structure representing a profile description
  */
  diet_profile_desc_t* mprofile;
};
#endif // SERVERUMS
