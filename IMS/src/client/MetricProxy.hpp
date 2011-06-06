/**
 * \file MetricProxy.hpp
 * \brief This file defines the metrics interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __METRICPROXY__HH__
#define __METRICPROXY__HH__

#include "IMS_Data.hpp"
#include "SessionProxy.hpp"

/**
 * \class MetricProxy
 * \brief The metric proxy class implementation
 */
class MetricProxy {
public:
  /**
   * \brief Constructor
   * \param session: The session connected
   */
  MetricProxy(const SessionProxy& session);
  /**
   * \brief Destructor
   */
  ~MetricProxy();
  /**
   * \brief To set the update frequency of the history maker
   * \param freq: The new frequency to set
   */
  void
  setUpFreq(int freq);
  /**
   * \brief To get the update frequency of the history make
   */
  int
  getUpFreq();
protected:
private:
  /**
  * \brief The object to manipulate the session data
  */
  SessionProxy msessionProxy;

};

#endif
