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
      if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0) {
  
	//if the machine exists and it is not locked
	if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'\
	  and status=1").size() != 0) {  
	  
	  //To get the database number id of the machine
	  numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
	  //To get the database number id of the user
	  numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");
	
	  //if the local account does not exist
	  if (!exist(numMachine, numUser)) {  
	    //The sql code to insert the localAccount on the databa
	    mdatabaseVishnu->process(sqlInsert + "('"+numMachine+"', '"+numUser+"', '"+mlocalAccount->getAcLogin()+"', '\
	    "+mlocalAccount->getSshKeyPath()+"', '"+mlocalAccount->getHomeDirectory()+"')");
	  
	  } //END if the local account does not exist
	  else {
	    UMSVishnuException e (4, "The local account already exists");
	    throw e;
	  }
	} //End if the machine exists and it is not locked
	else {
	  UMSVishnuException e (4, "The machine does not exist or it is locked");
	  throw e;
	}
      }//if the session key is for the owner of the local account or the user is an admin
      else {
	UMSVishnuException e (4, "The userid is incompatible with the session key");
	throw e;
      }
    }//End if the user exists
    else {
      UMSVishnuException e (4, "The user is unknown");
      throw e;
    }
  }
  catch (SystemException& e) {
    throw e;
 }
  //delete machine;
  return 0;
}
 
int 
LocalAccountServer::update() {
  return 0;
}
 
int 
LocalAccountServer::deleteLocalAccount() {
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
  } 
  catch (SystemException& e) {
    throw e;
  }
  if (result->getNbTuples() != 0) {
    result->print();
    std::vector<std::string> tmp = result->get(0);  
    ii=tmp.begin();
    return (*ii);
  } 
  else {
    return "";
  }
}

bool 
LocalAccountServer::exist(std::string idmachine, std::string iduser) {
 
  try {
  return (getAttribut("where machine_nummachineid='\
  "+idmachine+"' and users_numuserid='"+iduser+"'").size() != 0);
  }
  catch (SystemException& e) {
    throw e;
  }
  
}
  
/*
UMS_Data::ListLocalAccounts  LocalAccountServer::list(SessionServer session, UMS_Data::ListLocalAccOptions  options)
{
	return 0;
}*/
