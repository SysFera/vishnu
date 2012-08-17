/**
  * \file ProcessCtlProxy.hpp
  * \brief This file contains the VISHNU ProcessCtlProxy class.
  * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
  * \date May 2011
  */

#ifndef _PROCESSCTL_PROXY_H
#define _PROCESSCTL_PROXY_H

#include "IMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class ProcessCtlProxy
 * \brief ProcessCtlProxy class implementation
 */
class ProcessCtlProxy {

  public:

   /**
  * \param session The object which encapsulates the session information
  * \brief Constructor
  */
  explicit ProcessCtlProxy( const SessionProxy& session);

  /**
  * \param session The object which encapsulates the session information
  * \param machineId The id of the machine
  * \brief Constructor
  */
  explicit ProcessCtlProxy( const SessionProxy& session,
                            const std::string& machineId);

  /**
  * \brief Function to to restart a SeD or a MA
  * \param options the options for the restart
  * \return raises an exception on error
  */
  int
  restart(const IMS_Data::SupervisorOp& options);

  /**
  * \brief Function to get the refresh period
  * \param process The data structure containing information about the process to stop
  * \param op option for the supervisor
  * \return raises an exception on error
  */
  int
  stop(const IMS_Data::SupervisorOp& op);

  /**
  * \brief Function to get the refresh period
  * \param loadShedType the type of the load shedding mode (SOFT or HARD)
  * \return raises an exception on error
  * \return raises an exception on error
  */
  int
  loadShed(IMS_Data::LoadShedType loadShedType,
           const IMS_Data::SupervisorOp& op);

  /**
    * \brief Destructor, raises an exception on error
    */
  ~ProcessCtlProxy();

  private:
  /**
   * \brief Call the FMS sed to cancel all
   */
  void
  cancelFMS();
  /**
   * \brief Call the TMS sed to cancel all
   */
  void
  cancelTMS();

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The object to manipulate the session data
  */
  SessionProxy msessionProxy;
  /**
  * \brief The id of the machine
  */
  std::string mmachineId;

};
#endif //_PROCESSCTL_PROXY_H
