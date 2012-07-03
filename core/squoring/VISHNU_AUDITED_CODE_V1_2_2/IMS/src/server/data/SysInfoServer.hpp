/**
 * \file SysInfoServer.hpp
 * \brief This file defines the metrics interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __SYSINFOSERVER__HH__
#define __SYSINFOSERVER__HH__

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"
#include "SessionServer.hpp"

using namespace std;

/**
 * \class SysInfoServer
 * \brief Class that links the database and the system information about the machines
 */
class SysInfoServer{
public:
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   */
  SysInfoServer(const UserServer session);
  /**
   * \brief Constructor
   * \param session: A session to use for the call
   * \param op: The option of the sysinfo
   */
  SysInfoServer(const UserServer session, IMS_Data::SysInfoOp op);
  /**
   * \brief Destructor
   */
  ~SysInfoServer();
  /**
   * \brief To get system information
   * \return A list of system info
   */
  IMS_Data::ListSysInfo_ptr
  getSysInfo();
  /**
   * \brief To set system information
   */
  void
  setSysInfo(IMS_Data::SystemInfo_ptr sys);
protected:
private:
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
  IMS_Data::SysInfoOp mop;

}; // end sysinfoserver

#endif
