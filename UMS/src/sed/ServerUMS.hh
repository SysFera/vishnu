/**
 * \file ServerUMS.hh
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#ifndef _SERVERUMS_H_
#define _SERVERUMS_H_

#include <string>
#include "internalApi.hh"
//#include "DIET_server.h"
#define NB_SRV 9


//static Database* mdatabaseVishnu;
  
static const char* SRV[NB_SRV] = {
  "sessionConnect", 
  "sessionReconnect", 
  "sessionClose",
  "userCreate",
  "userUpdate",
  "userDelete",
  "userPasswordChange",
  "userPasswordReset",
  "machineCreate"};
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
   * \brief To initialize the UMS server
   * \fn void init()
   */
   void
   init();
  /**
  * \brief Constructor, raises an exception on error
  * \fn ServerUMS(std::string cfg)
  * \param cfg The vishnu configuration filepath
  * \param vishnuid The id of the vishnu configuration registered in the database
  */  
  ServerUMS(std::string cfg, std::string vishnuid);
  /**
   * \brief Destructor, raises an exception on error
   * \fn ~ServerUMS()
   */
  ~ServerUMS();
  /**
  * \brief The id of the VISHNU configuration registered in the database
  */
  static std::string mvishnuid;
  
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
