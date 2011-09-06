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

/**
 * \class MetricServer
 * \brief Server that link the metrics and the database
 */
class MetricServer{
public:
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   * \param mail: The script to send mails
   */
  MetricServer(const UserServer session, string mail);
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   * \param op: Options for the get metric history call
   * \param mail: The script to send mails
   */
  MetricServer(const UserServer session, IMS_Data::MetricHistOp_ptr op, string mail);
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   * \param op: Options for the get current metric value call
   * \param mail: The script to send mails
   */
  MetricServer(const UserServer session, IMS_Data::CurMetricOp_ptr op, string mail);
  /**
   * \brief Destructor
   */
  ~MetricServer();
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
  unsigned int
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

  /**
   * \brief To add all the monitored types of metric in the database at a time t
   * \param set: A list containing the monitored
   * \param mid: The machine id of the machine to add the set
   */
  void
  addMetricSet(IMS_Data::ListMetric* set, string mid);


protected:
private:
  /**
   * \brief To send a mail 
   * \param val: The value of the metric
   * \param threshold: The value of the threshold
   * \param type: The type of the threshold
   * \param email: The email adress
   * \param uid: The user id of the user target
   * \param mid: The machine id of the machine where the threshold is reached
   */
  void
  sendMail(int val, int threshold, int type, string email, string uid, string mid);
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
  unsigned int mfreq;
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
   * \brief The process to send mails
   */
  string msendmail;
  /**
   * \brief The options for the history of metrics
   */
  IMS_Data::MetricHistOp_ptr mhop;
  /**
   * \brief The options for the current metrics
   */
  IMS_Data::CurMetricOp_ptr mcop;
};

#endif
