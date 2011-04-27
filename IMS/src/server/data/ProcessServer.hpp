/**
 * \file ProcessServer.hpp
 * \brief This file defines the processes interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __PROCESSSERVER__HH__
#define __PROCESSSERVER__HH__

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"
#include "SessionServer.hpp"

using namespace std;

class ProcessServer{
public:
  /**
   * \brief If the process is in an undefined state
   */
  static const int PUNDEF = 0;
  /**
   * \brief If the process is running
   */
  static const int PRUNNING = 1;
  /**
   * \brief If the process is down
   */
  static const int PDOWN = 2;
  /**
   * \brief If the process is deleted
   */
  static const int PDELETED = 3;
  /**
   * \brief Constructor
   */
  ProcessServer(const UserServer session);
  /**
   * \brief Constructor
   */
  ProcessServer(IMS_Data::ProcessOp_ptr op, const UserServer session);
  /**
   * \brief Destructor
   */
  ~ProcessServer();
  /**
   * \brief Function to list query information
   * \fn IMS_Data::ListProcesses* list()
   * \return The pointer to the ListOject containing list information
   * \return raises an exception on error
   */
  IMS_Data::ListProcesses*
  list();

  /**
   * \brief Function to get the name of the query command line
   * \fn  string getCommandName()
   * \return The the name of the query command line
   */
  string getCommandName();
  /**
   * \brief To update the database with a process connexion
   * \param proc: The process that has just connected
   * \return Success, an error code otherwise
   */
  int
  connectProcess(IMS_Data::Process_ptr proc);
  /**
   * \brief To update the database with a process disconnexion
   * \param proc: The process that has just disconnected
   * \return Success, an error code otherwise
   */
  int
  disconnectProcess(IMS_Data::Process_ptr proc);
  /**
   * \brief To update the database with the link between the diet and vishnu id
   * \param proc: The process that has just been authentified
   * \return Success, an error code otherwise
   */
  int
  authentifyProcess(IMS_Data::Process_ptr proc);
  /**
   * \brief To set a process as stopped
   * \param proc: The proc to flag as stopped
   * \return Success, an error code otherwise
   */
  int
  stopProcess(IMS_Data::Process_ptr proc);
  /**
   * \brief To get a vishnu machineid based on the hostname
   * \param hostname: The name of the hostname
   * \return The corresponding machine id
   */
  string
  getMidFromHost(string hostname);
  /**
   * \brief Return true if the process with a diet name Pname is of type IMS
   * \param Pname: DIET name of the process
   * \return True if Pname represents an IMS SeD
   */
  bool
  isIMSSeD(string Pname);

protected:
private:
  /**
   * \brief Position of the machine id in the machine table, starting at 0
   */
  static const int MIDPOS = 7;
  /**
   * \brief Position of the diet name in the process table, starting at 0
   */
  static const int NAMEPOS = 3;
  /**
   * \brief The list option
   */
  IMS_Data::ProcessOp_ptr mop;
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
};

#endif
