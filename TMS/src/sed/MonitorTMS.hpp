/**
 * \file MonitorTMS.hpp
 * \brief This file presents the implementation of the TMS Monitor
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef _MONITORTMS_H_
#define _MONITORTMS_H_

#include <string>
#include "internalApi.hpp"

/**
 * \class MonitorTMS
 * \brief This class describes the TMS Monitor
 */
class MonitorTMS {
public :
  /**
   * \brief Constructor, raises an exception on error
   * \fn MonitorTMS(int interval)
   * \param interval The interval in seconds to check the database
   */
  MonitorTMS(int interval = 1);

  /**
   * \brief Destructor
   * \fn ~MonitorTMS()
   */
  ~MonitorTMS();

   /**
   * \brief To initialize the TMS monitor with individual parameters instead of configuration file
   * \fn int init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword)
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbConfig The configuration of the database
   * \return raises an execption
   */
  void
  init(int vishnuId, DbConfiguration dbConfig, const std::string& machineId, const BatchType& batchType);

   /**
   * \brief To launch the TMS Monitor
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

  std::string mmachineId;
  BatchType mbatchType;
};
#endif // MONITORTMS
