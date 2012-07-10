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
#define NB_SRV 33
#include "DIET_server.h"
#include "UMSMapper.hpp"
#include "TMSMapper.hpp"
#include "FMSMapper.hpp"
#include "IMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "DbConfiguration.hpp"
#include "AuthenticatorConfiguration.hpp"

class SOCIDatabase;
class Authenticator;

static const char* SRV[NB_SRV] = {
  "sessionConnect",//0
  "sessionReconnect",//1
  "sessionClose",//2
  "userCreate",//3
  "userUpdate",//4
  "userDelete",//5
  "userPasswordChange",//6
  "userPasswordReset",//7
  "machineCreate",//8
  "machineUpdate",//9
  "machineDelete",//10
  "localAccountCreate",//11
  "localAccountUpdate",//12
  "localAccountDelete",//13
  "configurationSave",//14
  "configurationRestore",//15
  "optionValueSet",//16
  "optionValueSetDefault",//17
  "sessionList",//18
  "localAccountList",//19
  "machineList",//20
  "commandList",//21
  "optionValueList",//22
  "userList",//23
  "restore",//24
  "authSystemCreate",//25
  "authSystemUpdate",//26
  "authSystemDelete",//27
  "authSystemList",//28
  "authAccountCreate",//29
  "authAccountUpdate",//30
  "authAccountDelete",//31
  "authAccountList"};//32
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
  * \fn int getVishnuId()const
  * \return the path of the configuration file
  */
  int
  getVishnuId() const;

  /**
  * \brief To get the path to the sendmail script
  * \fn std::string getSendmailScriptPath() const
  * \return the path of the configuration file
  */
  std::string
  getSendmailScriptPath() const;

   /**
   * \brief To initialize the UMS server
   * \param vishnuId  The identifier of the vishnu instance
   * \param dbConfig  The configuration of the database
   * \param sendmailScriptPath The path to the script for sending emails
   * \param authenticatorConfig The configuration of the authenticator
   * \return an error code (0 if success and 1 if an error occurs)
   */
  int
  init(int vishnuId,
       DbConfiguration dbConfig,
       std::string sendmailScriptPath,
       AuthenticatorConfiguration authenticatorConfig);

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

  /**
   * \brief operator=
   */
  ServerUMS& operator=(const ServerUMS&);

  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The singleton reference
   */
  static ServerUMS *minstance;
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
  /**
  * \brief Instance of Database
  */
  static SOCIDatabase *mdatabaseVishnu;
  /**
  * \brief Instance of UMSMapper
  */
  static UMSMapper *mmapper;
  /**
  * \brief Instance of UMSMapper
  */
  static TMSMapper *mmapperTMS;
  /**
  * \brief Instance of UMSMapper
  */
  static FMSMapper *mmapperFMS;
  /**
  * \brief Instance of UMSMapper
  */
  static IMSMapper *mmapperIMS;
  /**
  * \brief Instance of Authenticator
  */
  static Authenticator *mauthenticator;
};
#endif // SERVERUMS
