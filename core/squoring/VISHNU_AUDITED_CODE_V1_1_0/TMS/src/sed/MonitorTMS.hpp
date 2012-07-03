/**
 * \file MonitorTMS.hpp
 * \brief This file presents the implementation of the TMS Monitor
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date April
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
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbConfig The configuration of the database
   * \param machineId The machine identifier
   * \param batchType The batch scheduler type
   * \return raises an execption
   */
  void
  init(int vishnuId, DbConfiguration dbConfig, const std::string& machineId, const BatchType& batchType);

   /**
   * \brief To launch the TMS Monitor
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
  SOCIDatabase *mdatabaseVishnu;

  /**
   * \brief The machine identifier
   */
  std::string mmachineId;

  /**
   * \brief The batch scheduler type 
   */
  BatchType mbatchType;
};
#endif // MONITORTMS
