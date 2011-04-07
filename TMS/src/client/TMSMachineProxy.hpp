/**
  * \file TMSMachineProxy.hpp
  * \brief This file contains the VISHNU TMSMachineProxy class.
  * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
  * \date March 2011
  */

#ifndef _TMSMACHINE_PROXY_H
#define _TMSMACHINE_PROXY_H

#include "TMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class TMSMachineProxy
 * \brief TMSMachineProxy class implementation
 */
class TMSMachineProxy {

  public:

  /**
  * \param session The object which encapsulates the session information
  * \param machineId The id of the machine
  * \brief Constructor
  */
  explicit TMSMachineProxy( const SessionProxy& session,
                            const std::string& machineId);

  /**
  * \brief Function to get the list of queues
  * \return A ListQueues data structure
  */
  TMS_Data::ListQueues_ptr
  getMachineQueues();
  /**
  * \brief Function to set a refresh period
  * \param refreshPeriod the refresh period
  * \return raises an exception on error
  */
  void
  setMachineRefreshPeriod(int refreshPeriod);

  /**
  * \brief Function to get the refresh period
  * \return the refresh period value
  */
  int
  getMachineRefreshPeriod();

  /**
    * \brief Destructor, raises an exception on error
    */
  ~TMSMachineProxy();

  private:

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
  /**
  * \brief The refresh period value
  */
  int mrefreshPeriod;

};
#endif //_TMSMACHINE_PROXY_H
