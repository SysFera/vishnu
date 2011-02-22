/**
* \file LocalAccountServer.cpp
* \brief This file implements the Class which manipulates VISHNU localAccount data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "LocalAccountServer.hpp"

/**
* \brief Constructor
* \fn LocalAccountServer(LocalAccount account, SessionServer session)
* \param account The LocalAccount data structure
* \param session The object which encapsulates session data
*/
LocalAccountServer::LocalAccountServer(UMS_Data::LocalAccount*& account, SessionServer& session):
mlocalAccount(account), msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU localAccount
* \fn int add() 
* \return raises an exception on error
*/
int
LocalAccountServer::add() {
  
  std::string numMachine;
  std::string numUser;
  std::string sqlInsert = "insert into account (machine_nummachineid, users_numuserid, \
  aclogin, sshpathkey, home) values ";
 
  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    
    //Creation of the object machine
    UMS_Data::Machine *machine = new UMS_Data::Machine();
    machine->setMachineId(mlocalAccount->getMachineId());
    MachineServer machineServer = MachineServer(machine);
    
    //if the user exists 
    if (userServer.exist()) {
      //if the session key is for the owner of the local account or the user is an admin
      if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
	userServer.isAdmin()){
	//if the machine exists and it is not locked
	if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'\
	  and status=1").size() != 0) {  
	  //To get the database number id of the machine
	  numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
	  //To get the database number id of the user
	  numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");
	
	  //if the local account does not exist
	  if (!exist(numMachine, numUser)) {  
	    
	    //The sql code to insert the localAccount on the database
	    mdatabaseVishnu->process(sqlInsert + "('"+numMachine+"', '"+numUser+"', '"+mlocalAccount->getAcLogin()+"', '\
	    "+mlocalAccount->getSshKeyPath()+"', '"+mlocalAccount->getHomeDirectory()+"')");
	    
	    msshpublickey = machineServer.getAttribut("where \
	    machineid='"+mlocalAccount->getMachineId()+"'", "sshpublickey");
	    
	  }//END if the local account does not exist
	  else {
	    UMSVishnuException e (ERRCODE_LOCAL_ACCOUNT_EXIST);
	    throw e;
	  }
	} //End if the machine exists and it is not locked
	else {
	  UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
	  throw e;
	}
      }//End if the session key is for the owner of the local account or the user is an admin
      else {
	UMSVishnuException e (ERRCODE_INCOMPATIBLE_USER_SESSION);
	throw e;
      }
    }//End if the user exists
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
    //delete machine;
  }
  catch (VishnuException& e) {
    throw;
  }
  
  return 0;
}

/**
* \brief Function to update a VISHNU localAccount
* \fn int update() 
* \return raises an exception on error
*/
int 
LocalAccountServer::update() {
  std::string numMachine;
  std::string numUser;
  std::string sqlCommand = "";
  
  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    
    //Creation of the object machine
    UMS_Data::Machine *machine = new UMS_Data::Machine();
    machine->setMachineId(mlocalAccount->getMachineId());
    MachineServer machineServer = MachineServer(machine);
    
    //if the user exists 
    if (userServer.exist()) {
      //if the session key is for the owner of the local account or the user is an admin
      if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
	userServer.isAdmin()){
  
	//if the machine exists and it is not locked
	if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'\
	  and status=1").size() != 0) {  
	  
	  //To get the database number id of the machine
	  numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
	  //To get the database number id of the user
	  numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");
	
	  //if the local account exists
	  if (exist(numMachine, numUser)) {  
	    
	    //if a new acLogin has been defined
	    if (mlocalAccount->getAcLogin().size() != 0) {
	    sqlCommand.append("UPDATE account SET aclogin='"+mlocalAccount->getAcLogin()+"'\
	    where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");	  
	    }
	   
	    //if a new sshpathkey has been defined
	    if (mlocalAccount->getSshKeyPath().size() != 0) {
	    sqlCommand.append("UPDATE account SET sshpathkey='"+mlocalAccount->getSshKeyPath()+"'\
	    where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");	  
	    }
	    
	    //if a new home directory has been defined
	    if (mlocalAccount->getHomeDirectory().size() != 0) {
	    sqlCommand.append("UPDATE account SET home='"+mlocalAccount->getHomeDirectory()+"'\
	    where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");	  
	    }
	    
	    std::cout <<"SQL COMMAND:"<<sqlCommand;
	    mdatabaseVishnu->process(sqlCommand.c_str());
	  
	  }//END if the local account exists
	  else {
	    UMSVishnuException e (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
	    throw e;
	  }
	} //End if the machine exists and it is not locked
	else {
	  UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
	  throw e;
	}
      }//if the session key is for the owner of the local account or the user is an admin
      else {
	UMSVishnuException e (ERRCODE_INCOMPATIBLE_USER_SESSION);
	throw e;
      }
    }//End if the user exists
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
    //delete machine;
  }
  catch (VishnuException& e) {
    throw;
 }
  
  return 0;
}

/**
* \brief Function to delete a VISHNU localAccount
* \fn int deleteLocalAccount() 
* \return raises an exception on error
*/
int 
LocalAccountServer::deleteLocalAccount() {
  
  std::string numMachine;
  std::string numUser;
  
  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    
    //Creation of the object machine
    UMS_Data::Machine *machine = new UMS_Data::Machine();
    machine->setMachineId(mlocalAccount->getMachineId());
    MachineServer machineServer = MachineServer(machine);
    
    //if the user exists 
    if (userServer.exist()) {
      //if the session key is for the owner of the local account or the user is an admin
      if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
	userServer.isAdmin()){
  
	//if the machine exists and it is not locked
	if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'\
	  and status=1").size() != 0) {  
	  
	  //To get the database number id of the machine
	  numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
	  //To get the database number id of the user
	  numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");
	
	  //if the local account exists
	  if (exist(numMachine, numUser)) {  
	    
	    //To remove the local account from the database
	    mdatabaseVishnu->process("DELETE FROM account \
	    where machine_nummachineid="+numMachine+" and users_numuserid="+numUser);
	         
	  }//END if the local account exists
	  else {
	    UMSVishnuException e (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
	    throw e;
	  }
	} //END if the machine exists and it is not locked
	else {
	  UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
	  throw e;
	}
      }//END if the session key is for the owner of the local account or the user is an admin
      else {
	UMSVishnuException e (ERRCODE_NO_ADMIN);
	throw e;
      }
    }//End if the user exists
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
  }
  catch (VishnuException& e) {
    throw;
 }
 
  return 0;
}
/**
* \fn ~LocalAccountServer
* \brief Destructor
*/ 
LocalAccountServer::~LocalAccountServer() {
  //delete mlocalAccount;
}
/**
* \brief Function to get localAccount information
* \fn UMS_Data::LocalAccount getData()
* \return  The LocalAccount data structure
*/ 
UMS_Data::LocalAccount *
LocalAccountServer::getData() {
  return mlocalAccount;
}
/**
* \brief Function to get localAccount information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/  
std::string 
LocalAccountServer::getAttribut(std::string condition, std::string attrname) {
  DatabaseResult* result;
  std::vector<std::string>::iterator ii;
  
  std::string sqlCommand("SELECT "+attrname+" FROM account "+condition);
  std::cout <<"SQL COMMAND:"<<sqlCommand;
    
  try {
    result = mdatabaseVishnu->getResult(sqlCommand.c_str());
    return result->getFirstElement();
  } 
  catch (VishnuException& e) {
    throw;
  }
  
}

/**
* \brief Function to check localAccount on database
* \fn bool exist()
* \return true if the localAccount exists else false
*/
bool 
LocalAccountServer::exist(std::string idmachine, std::string iduser) {
 
  try {
    return (getAttribut("where machine_nummachineid="+idmachine+" and users_numuserid="+iduser).size() != 0);
  }
  catch (VishnuException& e) {
    throw;
  }
  
}
  
/**
* \brief Function to get the content of the public ssh generated
* \fn std::string getPublicKey()
* \return The content of the ssh public key generated
*/
std::string
LocalAccountServer::getPublicKey() {
  return msshpublickey;
}
    