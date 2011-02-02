#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "POSTGREDatabase.hh"
#include "DbFactory.hh"
#include "DatabaseResult.hh"

#include "MachineClientServer.hh"
#include <exception>
#include "SystemException.hh"


MachineClientServer::MachineClientServer(std::string sshKey, std::string host) {
  	mmachineSSHKey = sshKey;
	mhostname = host;
	mdatabaseVishnu = factory.getDatabaseInstance();
}
 
int MachineClientServer::recordMachineClient()
{
    
    try {
    std::string sqlCmd = std::string("insert into clmachine (sshkey, name) values ('"); //TODO: Faire une fonction pr construire les cmds sql
    sqlCmd.append(mmachineSSHKey+"','");
    sqlCmd.append(mhostname+"')");
    } catch (SystemException& e) {
	throw e;
    }
    
    return 0;
}
 
int MachineClientServer::getId()
{	
	
	/*DatabaseResult* result;
	std::string sqlCommand("SELECT numclmachineid FROM clmachine where sskey='");
	sqlCommand.append(mmachineSSHKey+"')");
	
	
	std::cout <<"SQL COMMAND:"<<sqlCommand;
        
	try {
	    result = mdatabaseVishnu->getResult(sqlCommand.c_str());
	
	} catch (SystemException& e) {
	throw e;
	}
	
	
	std::cout << "Nb résulats:" << result->getNbTuples() << std::endl;
	 
	if (result->getNbTuples() != 0) {
	      result->print();
	      std::vector<std::string> obj = result->get(0);
	     // obj.
	      return; 
	} 
	else {
	    SystemException e(4, "The machine "+mhostname+"SSH Key is unknown");
	    throw e;
	}

	
	*/
	return 0;
}
 
std::string MachineClientServer::getSSHKey() const{

	return mmachineSSHKey;
}
 
std::string MachineClientServer::getHost() const{
	return mhostname;
}
