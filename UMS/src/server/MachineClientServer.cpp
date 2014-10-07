/**
* \file MachineClientServer.cpp
* \brief This file implements the Class which manipulates the VISHNU client machine data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include <boost/scoped_ptr.hpp>
#include "MachineClientServer.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "utilVishnu.hpp"

/**
 * \brief Constructor
 * \param sshKey The ssh key of the client machine
 * \param host The name of the client host
 */
MachineClientServer::MachineClientServer(const std::string& sshKey,
                                         const std::string& host)
  : mmachineSSHKey(sshKey), mhostname(host) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
 * \brief Function to record the client machine on the database
 * \return raises an exception on error
 */
int
MachineClientServer::recordMachineClient() {

  //If the machine is not on the database
  if (!exist()) {
    std::string sqlCmd = std::string("insert into clmachine (sshkey, name) values ('");
    sqlCmd.append(mdatabaseVishnu->escapeData(mmachineSSHKey)+"','");
    sqlCmd.append(mdatabaseVishnu->escapeData(mhostname)+"')");

    mdatabaseVishnu->process(sqlCmd);

  } //End if the machine is not on the database
  else {
  }
  return 0;
}

/**
 * \brief Function to get the id number of the client machine
 * \return raises an exception on error
 */
std::string
MachineClientServer::getId() {

  std::string sqlCommand("SELECT numclmachineid FROM clmachine where sshkey='");
  sqlCommand.append(mdatabaseVishnu->escapeData(mmachineSSHKey)+ "' and name='");
  sqlCommand.append(mdatabaseVishnu->escapeData(mhostname)+"'");

  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand));
  return result->getFirstElement();
}
/**
 * \brief Function to get the ssh key of the client machine
 * \return raises an exception on error
 */
std::string
MachineClientServer::getSSHKey() const{
  return mmachineSSHKey;
}

/**
 * \brief Function to get the hostname of the client machine
 * \return raises an exception on error
 */
std::string
MachineClientServer::getHost() const{
  return mhostname;
}
/**
* \brief Destructor
*/
MachineClientServer::~MachineClientServer() {
}

/**
* \brief Function to check the client machine on the database
* \return true if the machine exists on the database else false
*/
bool
MachineClientServer::exist(){
  return (getId().size() != 0);
}
