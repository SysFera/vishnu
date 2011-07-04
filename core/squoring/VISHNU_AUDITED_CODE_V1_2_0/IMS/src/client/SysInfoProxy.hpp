/**
  * \file SysInfoProxy.hpp
  * \brief This file contains the VISHNU SysInfoProxy class.
  * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
  * \date March 2011
  */

#ifndef _SYSINFO_PROXY_H
#define _SYSINFO_PROXY_H

#include "IMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class SysInfoProxy
 * \brief SysInfoProxy class implementation
 */
class SysInfoProxy {

  public:

  /**
  * \param session The object which encapsulates the session information
  * \brief Constructor
  */
  explicit SysInfoProxy(const SessionProxy& session);

 /**
  * \brief Function to set a system information
  * \param systemInfo the system information
  * \return raises an exception on error
  */
  void
  setSystemInfo(IMS_Data::SystemInfo systemInfo);

  /**
  * \brief Function to get a system information
  * \param options the options data structure to get system information
  * \param listSysInfo the list of system information returned
  * \return raises an exception on error
  */
  int
  getSystemInfo(IMS_Data::ListSysInfo& listSysInfo,
                const IMS_Data::SysInfoOp& options);

  /**
    * \brief Destructor, raises an exception on error
    */
  ~SysInfoProxy();

  private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The object to manipulate the session data
  */
  SessionProxy msessionProxy;

};
#endif //_SYSINFO_PROXY_H
