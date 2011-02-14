/**
* \file MachineClientServer.cpp
* \brief This file implements the Class which manipulates the VISHNU client machine data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "MachineClientServer.hh"
/**
* \brief Constructor
* \fn MachineClientServer(std::string sshKey, std::string host)
* \param sshKey The ssh key of the client machine
* \param host The name of the client host
*/
MachineClientServer::MachineClientServer(std::string sshKey, std::string host) {
  DbFactory factory;
  mmachineSSHKey = sshKey;
  mhostname = host;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to record the client machine on the database
* \fn int recordMachineClient()
* \return raises an exception on error
*/
int MachineClientServer::recordMachineClient() {
  
  //If the machine is not on the database
  if (!exist()) {
    std::string sqlCmd = std::string("insert into clmachine (sshkey, name) values ('");
    sqlCmd.append(mmachineSSHKey+"','");
    sqlCmd.append(mhostname+"')");
    std::cout <<"SQL COMMAND:"<<sqlCmd;

    try {
      mdatabaseVishnu->process(sqlCmd.c_str());
    } 
    catch (SystemException& e) {
      throw e;
    }
  } //End if the machine is not on the database 
  else {
    std::cout << "The machine is already registered";
  } 
  return 0;
}

/**
* \brief Function to get the id number of the client machine
* \fn std::string getId()
* \return raises an exception on error
*/
std::string
MachineClientServer::getId() {	
    
  DatabaseResult* result;
  std::vector<std::string>::iterator ii;

  std::string sqlCommand("SELECT numclmachineid FROM clmachine where sshkey='");
  sqlCommand.append(mmachineSSHKey+ "' and name='");
  sqlCommand.append(mhostname+"'");

  std::cout <<"SQL COMMAND:"<<sqlCommand;
  try {
    result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } 
  catch (SystemException& e) {
    throw e;
  }
  if (result->getNbTuples() != 0) {  
    result->print();
    std::vector<std::string> tmp = result->get(0);
    ii=tmp.begin();
    return *ii;
  } 
  else {
    return "";
  }
}
/**
* \brief Function to get the ssh key of the client machine
* \fn std::string getSSHKey()
* \return raises an exception on error
*/
std::string MachineClientServer::getSSHKey() const{
  return mmachineSSHKey;
}

/**
* \brief Function to get the hostname of the client machine
* \fn std::string getHost()
* \return raises an exception on error
*/  
std::string MachineClientServer::getHost() const{
  return mhostname;
}
/**
* \fn ~MachineClientServer()
* \brief Destructor
*/
MachineClientServer::~MachineClientServer() {
}

/**
* \brief Function to check the client machine on the database
* \fn bool exist()
* \return true if the machine exists on the database else false
*/
bool MachineClientServer::exist(){
  try {
    return (getId().size() != 0);
  } 
  catch (SystemException& e) {
    throw e;
  }
}

