/**
* \file ConfigurationServer.cpp
* \brief This file implements the Class which manipulates VISHNU configuration data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "ConfigurationServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"

/**
* \brief Constructor
* \fn ConfigurationServer(SessionServer session)
* \param session The object which encapsulates session data
*/
ConfigurationServer::ConfigurationServer(SessionServer session):msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  mconfiguration = NULL;
 
}
/**
* \brief Constructor
* \fn ConfigurationServer(Configuration*& configuration, SessionServer session);
* \param configuration The Configuration data structure
* \param session The object which encapsulates session data
*/  
ConfigurationServer::ConfigurationServer(UMS_Data::Configuration*& configuration, SessionServer session) 
:mconfiguration(configuration), msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to save a VISHNU configuration
* \fn int save() 
* \return raises an exception on error
*/
int 
ConfigurationServer::save() {
  
  DatabaseResult *ListofUsers;
  DatabaseResult *ListofMachines;
  DatabaseResult *ListofLocalAccount;

  std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users \
  where not userid='vishnu_user' and not userid='vishnu_db_admin'";

  std::string sqlListofMachines = "SELECT machineid, name, site, status, sshpublickey, lang, description from machine, description \
  where machine.nummachineid = description.machine_nummachineid";

  std::string sqlListofLocalAccount = "SELECT machineid, userid, aclogin, sshpathkey, home \
  from account, machine, users where account.machine_nummachineid=machine.nummachineid and \
  account.users_numuserid=users.numuserid";

  std::vector<std::string>::iterator ii;
  std::vector<std::string> results;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mconfiguration= ecoreFactory->createConfiguration();
 

  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists 
    if (userServer.exist()) {
      //if the user is an admin
      if (userServer.isAdmin()) {
	
	//To get the list of users from the database
        ListofUsers = mdatabaseVishnu->getResult(sqlListofUsers.c_str());
	
	if (ListofUsers->getNbTuples() != 0){ 
	  for (size_t i = 0; i < ListofUsers->getNbTuples(); ++i) {
	    results.clear();
	    results = ListofUsers->get(i);
	    ii = results.begin();
	      
	    UMS_Data::User_ptr user = ecoreFactory->createUser();
	    user->setUserId(*ii);
	    user->setPassword(*(++ii));
	    user->setFirstname(*(++ii));
	    user->setLastname(*(++ii));
	    user->setPrivilege(convertToInt(*(++ii)));
	    user->setEmail(*(++ii));
	    user->setStatus(convertToInt(*(++ii)));
	    mconfiguration->getListConfUsers().push_back(user);
	   }
	}
	
	//To get the list of machines from the database
	ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str());
	
	if (ListofMachines->getNbTuples() != 0){ 
	  for (size_t i = 0; i < ListofMachines->getNbTuples(); ++i) {
	    results.clear();
	    results = ListofMachines->get(i);
	    ii = results.begin();
	  
	    UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
	    machine->setMachineId(*ii);
	    std::cout << machine->getMachineId() << std::endl;
	    machine->setName(*(++ii));
	    std::cout << machine->getName() << std::endl;
	    machine->setSite(*(++ii));
	    std::cout << machine->getSite() << std::endl;
	    machine->setStatus(convertToInt(*(++ii)));
	    std::cout << machine->getStatus() << std::endl;
	    machine->setSshPublicKey(*(++ii));
	    std::cout << machine->getSshPublicKey() << std::endl;
	    machine->setLanguage(*(++ii));
	    std::cout << machine->getLanguage() << std::endl;
	    machine->setMachineDescription(*(++ii));
	    std::cout << machine->getMachineDescription() << std::endl;
	    mconfiguration->getListConfMachines().push_back(machine);
	   }
	}
	
	
	//To get the list of local accounts from the database
	ListofLocalAccount = mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str());
	
	if (ListofLocalAccount->getNbTuples() != 0){ 
	  for (size_t i = 0; i < ListofLocalAccount->getNbTuples(); ++i) {
	    results.clear();
	    results = ListofLocalAccount->get(i);
	    ii = results.begin();
	      
	    UMS_Data::LocalAccount_ptr localAccount = ecoreFactory->createLocalAccount();
	    localAccount->setMachineId(*ii);
	    localAccount->setUserId(*(++ii));
	    localAccount->setAcLogin(*(++ii));
	    localAccount->setSshKeyPath(*(++ii));
	    localAccount->setHomeDirectory(*(++ii));
	    mconfiguration->getListConfLocalAccounts().push_back(localAccount);
	  }
	}	
      } //End if the user is an admin
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
* \brief Function to restore a VISHNU configuration
* \fn int restore() 
* \return raises an exception on error
*/
int ConfigurationServer::restore() {
  std::string sqlcode = "";
  std::string sqlCodeDescMachine = "";
  std::string sqlcodeLocalAccount = "";
  
  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists 
    if (userServer.exist()) {
      //if the user is an admin
      if (userServer.isAdmin()) {
	
	
	mdatabaseVishnu->process("DELETE FROM users where not userid='vishnu_user' and not userid='vishnu_db_admin';\
	DELETE FROM machine; DELETE FROM account;");
	
	//To insert all users 
	for(int i = 0; i < mconfiguration->getListConfUsers().size(); i++) {
	  UMS_Data::User_ptr user = mconfiguration->getListConfUsers().get(i);
	  userServer.add(user);
	}
	
	//To get all machines
	for(int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
	  UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
	  sqlcode.append(machineToSql(machine));
	}
	
        std::cout << "SQL COMMAND:" << sqlcode << std::endl;
	
	//To insert machines
        mdatabaseVishnu->process(sqlcode.c_str());
	
	//To get machine description which needs the database number id
	for(int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
	  UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
	 //sqlcode.append(machineToSql(machine));
	 sqlCodeDescMachine.append(machineDescToSql(machine));
	}
	
	std::cout << "SQL COMMAND:" << sqlCodeDescMachine << std::endl;
	
	//To insert machines description
	mdatabaseVishnu->process(sqlCodeDescMachine.c_str());
	
	//To insert localAccount
	for(int i = 0; i < mconfiguration->getListConfLocalAccounts().size(); i++) {
	  UMS_Data::LocalAccount_ptr localAccount = mconfiguration->getListConfLocalAccounts().get(i);
	  LocalAccountServer localAccountServer = LocalAccountServer (localAccount, msessionServer);
	  localAccountServer.add(); 	 
	}
  
      } //End if the user is an admin
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
* \fn ~ConfigurationServer()
* \brief Destructor
*/ 
ConfigurationServer::~ConfigurationServer() {
  delete mconfiguration;
}
/**
* \brief Function to get Configuration data structure
* \fn UMS_Data::Configuration*  getData()
* \return  The LocalAccount data structure
*/ 
UMS_Data::Configuration*
ConfigurationServer::getData() {
  return mconfiguration;
}

/**
* \brief Function to get the sql code of users from a VISHNU configuration
* \fn std::string userToSql(UMS_Data::User_ptr user) 
* \return the sql code containing the sql code of users
*/
std::string
ConfigurationServer::userToSql(UMS_Data::User_ptr user) {
  
  std::string sqlInsert = "insert into users (vishnu_vishnuid, userid, pwd, firstname, lastname,\
  privilege, email, passwordstate, status) values ";
  
  return (sqlInsert + "(" + Vishnuid::mvishnuid+", \
	  '"+user->getUserId()+"','"+user->getPassword()+"','"
	  + user->getFirstname()+"','"+user->getLastname()+"',"+ 
	  convertToString(user->getPrivilege()) +",'"+user->getEmail() +"', \
	  0, "+convertToString(user->getStatus())+");");
}

/**
* \brief Function to get the sql code of machines from a VISHNU configuration
* \fn std::string machineToSql(UMS_Data::Machine_ptr machine)
* \return the sql code containing the sql code of machines
*/
std::string
ConfigurationServer::machineToSql(UMS_Data::Machine_ptr machine) {
  std::string sqlInsert = "insert into machine (vishnu_vishnuid, name, site, machineid, status, sshpublickey) values ";
  
  sqlInsert.append("("+Vishnuid::mvishnuid+",'"+machine->getName()+"\
  ','"+ machine->getSite()+"','"+machine->getMachineId()+"',\
  "+convertToString(machine->getStatus())+",'"+machine->getSshPublicKey() +"');");
  
  return sqlInsert;
}

/**
* \brief Function to get the sql code of machines description from a VISHNU configuration
* \fn std::string machineDescToSql(UMS_Data::Machine_ptr machine)
* \return the sql code containing the sql code of machines description
*/
std::string
ConfigurationServer::machineDescToSql(UMS_Data::Machine_ptr machine) {
  
  UMS_Data::Machine* machinetmp = new UMS_Data::Machine();
  MachineServer machineServer = MachineServer(machinetmp);
  
  try {
    return ("insert into description (machine_nummachineid, lang, \
    description) values \
    ("+machineServer.getAttribut("where machineid='"+machine->getMachineId()+"';")+",\
    '"+ machine->getLanguage()+"','"+machine->getMachineDescription()+"');");
    
  }
  catch (VishnuException& e) {
    throw;
  }
  
  delete machinetmp;
}
