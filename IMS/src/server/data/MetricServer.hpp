/**
 * \file MetricServer.hpp
 * \brief This file defines the metrics interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __METRICSERVER__HH__
#define __METRICSERVER__HH__

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"
#include "SessionServer.hpp"

using namespace std;

class MetricServer{
public:
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   */
  MetricServer(const UserServer session);
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   * \param op: Options for the get metric history call
   */
  MetricServer(const UserServer session, IMS_Data::MetricHistOp op);
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   * \param op: Options for the get current metric value call
   */
  MetricServer(const UserServer session, IMS_Data::CurMetricOp op);
  /**
   * \brief Destructor
   */
  ~MetricServer();
  /**
   * \brief To get the frequency value
   */
  int
  getUpFreq();
  /**
   * \brief To set the update frequency value
   * \param freq: The new frequency value
   */
  void
  setUpFreq(int freq);
  /**
   * \brief Update the mfreq member with the value from the database
   * \return The new mfreq value
   */
  int
  checkUpFreq();
  /**
   * \brief Return the current metric values, gotten directly with CoRI
   * \return The current list of metrics
   */
  IMS_Data::ListMetric*
  getCurMet();
  /**
   * \brief Return the old values of the metrics, gotten from the database
   * \return The old list of metrics
   * \param machineId: The vishnu id of the machine
   */
  IMS_Data::ListMetric*
  getHistMet(string machineId);

protected:
private:
  /**
   * \brief The position of the frequency in the vishnu table
   */
  static const int FREQPOS = 1;
  /**
   * \brief The id of vishnu
   */
  int mvishnuId;
  /**
   * \brief The update frequency
   */
  int mfreq;
  /**
   * \brief The session
   */
  UserServer msession;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabase;
  /**
   * \brief The name of the command
   */
  string mcommandName;
  /**
   * \brief The options for the history of metrics
   */
  IMS_Data::MetricHistOp mhistOp;
};

#endif
