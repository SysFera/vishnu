/**
 * \file ThresholdServer.hpp
 * \brief This file defines the metrics interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __THRESHOLDSERVER__HH__
#define __THRESHOLDSERVER__HH__

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"
#include "SessionServer.hpp"

using namespace std;

/**
 * \class ThresholdServer
 * \brief Server that link the thresholds and the database
 */
class ThresholdServer{
public:
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   */
  ThresholdServer(const UserServer session);
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   * \param op: The option of the threshold
   */
  ThresholdServer(const UserServer session, IMS_Data::ThresholdOp op);
  /**
   * \brief Destructor
   */
  ~ThresholdServer();
  /**
   * \brief To set a threshold
   * \param tree: A threshold
   */
  void
  setThreshold(IMS_Data::Threshold_ptr tree);
  /**
   * \brief To get the thresholds
   * \return A list of thresholds
   */
  IMS_Data::ListThreshold_ptr
  getThreshold();
protected:
private:
  /**
   * \brief To get the numerical userid and numerical machineid corresponding to a threshold
   * \param tree: The threshold
   * \param nuid: The numerical user id (out)
   * \param nmid: The numerical machine id (out)
   */
  void
  getUserAndMachine(IMS_Data::Threshold_ptr tree, string &nuid, string &nmid);
  /**
   * \brief Check if a threshold of this type on this machine is already defined
   * \param tree: The threshold
   * \return True if the threshold already exist in the database
   */
  bool
  checkExist(IMS_Data::Threshold_ptr tree);
  /**
   * \brief The id of vishnu
   */
  int mvishnuId;
  /**
   * \brief The session
   */
  UserServer msession;
  /**
  * \brief An instance of vishnu database
  */
  SOCIDatabase *mdatabase;
  /**
   * \brief Option for getting sysinfo
   */
  IMS_Data::ThresholdOp mop;
};

#endif // threshold server

