/**
  * \file ThresholdProxy.hpp
  * \brief This file contains the VISHNU SysInfoProxy class.
  * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
  * \date March 2011
  */

#ifndef _THRESHOLD_PROXY_H
#define _THRESHOLD_PROXY_H

#include "IMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class ThresholdProxy
 * \brief ThresholdProxy class implementation
 */
class ThresholdProxy {

  public:

  /**
  * \param session The object which encapsulates the session information
  * \brief Constructor
  */
  explicit ThresholdProxy(const SessionProxy& session);

 /**
  * \brief Function to set a system threshold
  * \param systemThreshold the system threshold
  * \return raises an exception on error
  */
  void
  setSystemThreshold(IMS_Data::Threshold systemThreshold);

  /**
  * \brief Function to get a system threshold
  * \param options the options data structure to get system threshold
  * \param listSysThreshold the list of system threshold returned
  * \return raises an exception on error
  */
  int
  getSystemThreshold(IMS_Data::ListThreshold& listSysThreshold,
                const IMS_Data::ThresholdOp& options);

  /**
    * \brief Destructor, raises an exception on error
    */
  ~ThresholdProxy();

  private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The object to manipulate the session data
  */
  SessionProxy msessionProxy;
};
#endif //_THRESHOLD_PROXY_H
