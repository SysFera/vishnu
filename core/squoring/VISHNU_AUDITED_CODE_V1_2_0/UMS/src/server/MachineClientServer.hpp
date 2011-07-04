/**
* \file MachineClientServer.hpp
* \brief This file presents the Class which manipulates the VISHNU client machine data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef MACHINE_CLIENT_SERVER_H
#define MACHINE_CLIENT_SERVER_H

#include <sstream>
#include <exception>
#include "VishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "Database.hpp"

/**
* \class MachineClientServer
* \brief MachineClientServer class implementation
*/
class MachineClientServer {
public:
  /**
  * \brief Constructor
  * \fn MachineClientServer(std::string sshKey, std::string host)
  * \param sshKey The ssh key of the client machine
  * \param host The name of the client host
  */
  MachineClientServer(std::string sshKey, std::string host);
  /**
  * \brief Function to record the client machine on the database
  * \fn int recordMachineClient()
  * \return raises an exception on error
  */
  int
  recordMachineClient();
  /**
  * \brief Function to get the id number of the client machine
  * \fn std::string getId()
  * \return raises an exception on error
  */
  std::string
  getId();
  /**
  * \brief Function to get the ssh key of the client machine
  * \fn std::string getSSHKey() const
  * \return raises an exception on error
  */
  std::string
  getSSHKey() const;
  /**
  * \brief Function to get the hostname of the client machine
  * \fn std::string getHost() const
  * \return raises an exception on error
  */
  std::string
  getHost() const;
  /**
  * \fn ~MachineClientServer()
  * \brief Destructor
  */
  ~MachineClientServer();

private:
   /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The ssh key of the client machine
  */
  std::string mmachineSSHKey;
  /**
  * \brief The hostname of the client machine
  */
  std::string mhostname;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
  * \brief Function to check the client machine on the database
  * \fn bool exist()
  * \return true if the machine exists on the database else false
  */
  bool exist();
};
#endif//MACHINE_CLIENT_SERVER_H
