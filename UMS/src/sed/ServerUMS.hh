/**
 * \file ServerUMS.hh
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#ifndef _SERVERUMS_H_
#define _SERVERUMS_H_

#include <string>
#include "DIET_server.h"

#define NB_SRV 3


//static Database* mdatabaseVishnu;
  
static const char* SRV[NB_SRV] = {
  "sessionConnect", 
  "sessionReconnect", 
  "sessionClose"};

  /**
   * \brief Function to solve the service SessionConnect 
   * \fn    void solveSessionConnect(diet_profile_t* pb);
   * \param pb is a structure which corresponds to the descriptor of a profile
   * \return raises an exception on error
   */
  int 
  solveSessionConnect(diet_profile_t* pb);
  /**
   * \brief Function to solve the service SessionReconnect 
   * \fn    void solveSessionReconnect
   * \param pb is a structure which corresponds to the descriptor of a profile
   * \return raises an exception on error
   */
  int 
  solveSessionReconnect(diet_profile_t* pb);
  /**
   * \brief Function to solve the service SessionClose 
   * \fn    virtual int createDatabase() = 0
   * \param pb is a structure which corresponds to the descriptor of a profile
   * \return raises an exception on error
   */
  int 
  solveSessionClose(diet_profile_t* pb);  
  
  
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
   * \brief Constructor, raises an exception on error
   * \fn ServerUMS(std::string cfg)
   * \param cfg The vishnu configuration filepath
   */
  ServerUMS(std::string cfg);
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
  std::string vishnucfg;
  /**
  * \brief Structure representing a profile description
  */
  diet_profile_desc_t* profile;
  
  /////////////////////////////////
  // Functions
  /////////////////////////////////
  

};
#endif // SERVERUMS
