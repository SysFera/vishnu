/*#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <sstream>

#include "POSTGREDatabase.hh"
#include "DbFactory.hh"
#include "DatabaseResult.hh"
*/
#include <sstream>
#include <exception>

#include "MachineClientServer.hh"
#include "SystemException.hh"


MachineClientServer::MachineClientServer(std::string sshKey, std::string host) {
  	mmachineSSHKey = sshKey;
	mhostname = host;
	mdatabaseVishnu = factory.getDatabaseInstance();
}
 
int MachineClientServer::recordMachineClient()
{
  
  if (!exist()) {
    //TODO: Voir la comparaison des chaines
    std::string sqlCmd = std::string("insert into clmachine (sshkey, name) values ('"); //TODO: Faire une fonction pr construire les cmds sql
    sqlCmd.append(mmachineSSHKey+"','");
    sqlCmd.append(mhostname+"')");
    std::cout <<"SQL COMMAND:"<<sqlCmd;
    
    try {
	mdatabaseVishnu->process(sqlCmd.c_str());
	} catch (SystemException& e) {
	throw e;
	}
    
    
  } else {
    std::cout << "The machine is already registered";
  } 
     
    return 0;
}
 
int MachineClientServer::getId() {	

 DatabaseResult* result;
 std::vector<std::string>::iterator ii;

 std::string sqlCommand("SELECT numclmachineid FROM clmachine where sshkey='");
 sqlCommand.append(mmachineSSHKey+ "' and name='");
 sqlCommand.append(mhostname+"'");

 std::cout <<"SQL COMMAND:"<<sqlCommand;
 try {
 result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 } catch (SystemException& e) {
 throw e;
 }

 if (result->getNbTuples() != 0) {
      // res = true;
      result->print();
      std::vector<std::string> tmp = result->get(0);
  
  ii=tmp.begin();
  std::istringstream str(*ii);
  int value;
  str>>value;
  std::cout << "Value: "<< value;
  return value;
    
 } // if  if (result->getNbTuples() != 0)
 else {
  return -1;
 }

//return 0;
}
 
std::string MachineClientServer::getSSHKey() const{

	return mmachineSSHKey;
}
 
std::string MachineClientServer::getHost() const{
	return mhostname;
}

bool MachineClientServer::exist(){
  
 DatabaseResult* result;
 std::string sqlCommand("SELECT * FROM clmachine where sshkey='");
 sqlCommand.append(mmachineSSHKey+ "' and name='");
 sqlCommand.append(mhostname+"'");
	
 std::cout <<"SQL COMMAND:"<<sqlCommand;
 try {
      result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 } 
 catch (SystemException& e) {
	throw e;
 }
 std::cout << "Nb résulats:" << result->getNbTuples() << std::endl;
	    //std::cout<<result->getNbTuples();
	    
  return (result->getNbTuples() != 0); 
}
