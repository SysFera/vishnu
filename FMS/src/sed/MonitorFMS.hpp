/**
 * \file MonitorFMS.hpp
 * \brief This file presents the implementation of the FMS Monitor
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date June
*/

#ifndef _MONITORFMS_H_
#define _MONITORFMS_H_

#include <string>
//#include "internalApiFMS.hpp"
#include "internalApiFMS.hpp"

/**
 * \class MonitorFMS
 * \brief This class describes the FMS Monitor
 */
class MonitorFMS {
public :
  /**
   * \brief Constructor, raises an exception on error
   * \param interval The interval in seconds to check the database
   */
  MonitorFMS(int interval = 1);

  /**
   * \brief Destructor
   * \fn ~MonitorFMS()
   */
  ~MonitorFMS();

   /**
   * \brief To initialize the FMS monitor with individual parameters instead of configuration file
   * \param vishnuId The password of the root user vishnu_user for the connection with the database
   * \param dbConfig The configuration of the database
   * \return raises an execption
   */
  void
  init(int vishnuId, DbConfiguration dbConfig);

   /**
   * \brief To launch the FMS Monitor
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
#endif // MONITORFMS
