/**
 * \file MonitorUMS.hpp
 * \brief This file presents the implementation of the UMS Monitor
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef _MONITORUMS_H_
#define _MONITORUMS_H_

#include <string>
#include "internalApi.hpp"

/**
 * \class MonitorUMS
 * \brief This class describes the UMS Monitor
 */
class MonitorUMS {
public :
  /**
   * \brief Constructor, raises an exception on error
   * \fn MonitorUMS(int interval)
   * \param interval The interval in seconds to check the database
   */
  MonitorUMS(int interval = 1);

  /**
   * \brief Destructor
   * \fn ~MonitorUMS()
   */
  ~MonitorUMS();

   /**
   * \brief To initialize the UMS monitor with individual parameters instead of configuration file
   * \fn int init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword)
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbType   The type of the database (POSTGREDB|ORACLEDB)
   * \param dbHost   The host of the database server
   * \param dbUsername The name of the database user on the server
   * \param dbPassword The password of the database user on the server
   * \return raises an execption
   */
  void
  init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword);

   /**
   * \brief To launch the UMS Monitor
   * \fn int run()
   * \return raises an exception
   */
  int
  run();

private :

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The interval in seconds to check the database
  */
  int minterval;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;

};
#endif // MONITORUMS
