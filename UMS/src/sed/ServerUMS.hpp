/**
 * \file ServerUMS.hpp
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
 */

#ifndef _SERVERUMS_H_
#define _SERVERUMS_H_

#include <string>
#include <map>
#include "DIET_client.h"
#include "UMSMapper.hpp"
#include "TMSMapper.hpp"
#include "FMSMapper.hpp"
#include "IMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "DbConfiguration.hpp"
#include "AuthenticatorConfiguration.hpp"
#include "SeD.hpp"
#include "UMSServices.hpp"

class Database;
class Authenticator;

/**
 * \class ServerUMS
 * \brief This class describes the UMS server
 */
class ServerUMS : public SeD {
public :

  /**
   * \brief To get the unique instance of the server
   */
  static ServerUMS*  getInstance();

  /**
   * \brief To get the vishnuId
   * \return the path of the configuration file
   */
  int
  getVishnuId() const;

  /**
   * \brief To get the path to the sendmail script
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
   */
  ~ServerUMS();

private:
  /**
   * \brief Init the ptr function map
   */
  void
  initMap();

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
   * \brief Instance of Database
   */
  static Database *mdatabaseVishnu;
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
