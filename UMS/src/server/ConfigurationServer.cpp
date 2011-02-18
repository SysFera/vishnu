
#include "ConfigurationServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"

ConfigurationServer::ConfigurationServer(SessionServer session):msessionServer(session)
{
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  //mecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mconfiguration = NULL;//mecoreFactory->createConfiguration();
 
}
 
ConfigurationServer::ConfigurationServer(UMS_Data::Configuration*& configuration, SessionServer session) 
:mconfiguration(configuration), msessionServer(session)
{
  //mecoreFactory = mecoreFactory->createConfiguration();
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  
}
 
int 
ConfigurationServer::save() {
  
DatabaseResult *ListofUsers;
DatabaseResult *ListofMachines;
DatabaseResult *ListofLocalAccount;

std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users \
where not userid='vishnu_user' and not userid='vishnu_db_admin'";

std::string sqlListofMachines = "SELECT machineid, name, site, status, lang, description from machine, description \
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
	  for (int i = 0; i < static_cast <int> (ListofUsers->getNbTuples()); ++i) {
	    results.clear();
	    results = ListofUsers->get(i);
	    ii = results.begin();
	      //for(ii=ii+2;ii!=tmp.end();ii++){
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
	  for (int i = 0; i < static_cast <int> (ListofMachines->getNbTuples()); ++i) {
	    results.clear();
	    results = ListofMachines->get(i);
	    ii = results.begin();
	      //for(ii=ii+2;ii!=tmp.end();ii++){
		UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
		machine->setMachineId(*ii);
		std::cout << machine->getMachineId() << std::endl;
		machine->setName(*(++ii));
		std::cout << machine->getName() << std::endl;
		machine->setSite(*(++ii));
		std::cout << machine->getSite() << std::endl;
		machine->setStatus(convertToInt(*(++ii)));
		std::cout << machine->getStatus() << std::endl;
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
	  for (int i = 0; i < static_cast <int> (ListofLocalAccount->getNbTuples()); ++i) {
	    results.clear();
	    results = ListofLocalAccount->get(i);
	    ii = results.begin();
	      //for(ii=ii+2;ii!=tmp.end();ii++){
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
	UMSVishnuException e (4, "The user is not an admin");
	throw e;
      }
    }//End if the user exists
    else {
      UMSVishnuException e (4, "The user is unknown");
      throw e;
    }
  }
  catch (SystemException& e) {
    throw;
  }  
  return 0;
}
 
int ConfigurationServer::restore() {
  std::string sqlcode = "";
  std::string sqlCodeDescMachine = "";
  
  
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
	
	for(int i = 0; i < mconfiguration->getListConfUsers().size(); i++) {
	  UMS_Data::User_ptr user = mconfiguration->getListConfUsers().get(i);
	  sqlcode.append(userToSql(user));
	}
	
	for(int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
	  UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
	 sqlcode.append(machineToSql(machine));
	 //sqlCodeDescMachine.append(machineDescToSql(machine));
	}
	
	
	
        std::cout << "SQL COMMAND:" << sqlcode << std::endl;
	//insertion of the users and machines
        mdatabaseVishnu->process(sqlcode.c_str());
	
	for(int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
	  UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
	 //sqlcode.append(machineToSql(machine));
	 sqlCodeDescMachine.append(machineDescToSql(machine));
	}
	
	std::cout << "SQL COMMAND:" << sqlCodeDescMachine << std::endl;
	//insertion of the machine descriptions
	mdatabaseVishnu->process(sqlCodeDescMachine.c_str());
	
	for(int i = 0; i < mconfiguration->getListConfLocalAccounts().size(); i++) {
	  UMS_Data::LocalAccount_ptr localAccount = mconfiguration->getListConfLocalAccounts().get(i);
	  
	  //UMS_Data::LocalAccount_ptr *localAccountCopy = new UMS_Data::LocalAccount();
	  
	  
	 //sqlcode.append(machineToSql(machine));
	 //sqlcode.append(localAccountToSql(localAccount));
	 
	 LocalAccountServer localAccountServer = LocalAccountServer (localAccount, msessionServer);
	 //mlocalAccountServer = new LocalAccountServer (localAccount, msessionServer);
	 localAccountServer.add(); 
	 /*LocalAccountServer localAccountServer = LocalAccountServer (localAccount);
	 localAccountServer.add();
	 */
	}
	//delete localAccountServer;
	
	
	
  
      } //End if the user is an admin
      else {
	UMSVishnuException e (4, "The user is not an admin");
	throw e;
      }
    }//End if the user exists
    else {
      UMSVishnuException e (4, "The user is unknown");
      throw e;
    }
  }
  catch (SystemException& e) {
    throw;
  }
  
  
  return 0;
}
 
 
 
ConfigurationServer::~ConfigurationServer() {
  delete mconfiguration;
}
 
UMS_Data::Configuration*
ConfigurationServer::getConfiguration()
{
  return mconfiguration;
}

std::string
ConfigurationServer::userToSql(UMS_Data::User_ptr user) {
  
  std::string sqlInsert = "insert into users (vishnu_vishnuid, userid, pwd, firstname, lastname,\
  privilege, email, passwordstate, status) values ";
  
  return (sqlInsert + "(" + Vishnuid::mvishnuid+", \
	  '"+user->getUserId()+"','"+user->getPassword()+"','\
	  "+ user->getFirstname()+"','"+user->getLastname()+"',"+ 
	  convertToString(user->getPrivilege()) +",'"+user->getEmail() +"', \
	  0, "+convertToString(user->getStatus())+");");
}


std::string
ConfigurationServer::machineToSql(UMS_Data::Machine_ptr machine) {
  std::string sqlInsert = "insert into machine (vishnu_vishnuid, name, site, machineid, status) values ";
  
  sqlInsert.append("("+Vishnuid::mvishnuid+",'"+machine->getName()+"\
  ','"+ machine->getSite()+"','"+machine->getMachineId()+"',"+convertToString(machine->getStatus())+");");
  
  
  
  /*sqlInsert.append("insert into description (machine_nummachineid, lang, \
  description) values \
  ("+machineServer.getAttribut("where machineid='"+machine->getMachineId()+"'")+",\
  '"+ machine->getLanguage()+"','"+machine->getMachineDescription()+"');");*/
  
  return sqlInsert;
}

std::string
ConfigurationServer::machineDescToSql(UMS_Data::Machine_ptr machine) {
  
  //TODO: faire contructeur par défaut dans machine
  UMS_Data::Machine* machinetmp = new UMS_Data::Machine();
  MachineServer machineServer = MachineServer(machinetmp);
  
  try {
    return ("insert into description (machine_nummachineid, lang, \
    description) values \
    ("+machineServer.getAttribut("where machineid='"+machine->getMachineId()+"';")+",\
    '"+ machine->getLanguage()+"','"+machine->getMachineDescription()+"');");
  }
  catch (SystemException& e) {
    throw;
  }
  
}
  
std::string
localAccountToSql(UMS_Data::LocalAccount_ptr localAccount) {
 
  std::string sqlInsert = "insert into account (machine_nummachineid, users_numuserid, \
  aclogin, sshpathkey, home) values ";
  
  //TODO: faire contructeur par défaut dans machine
  UMS_Data::Machine* machinetmp = new UMS_Data::Machine();
  MachineServer machineServer = MachineServer(machinetmp);
  
  UserServer userServer = UserServer("","");
  
  //To get the database number id of the machine
  std::string numMachine = machineServer.getAttribut("where machineid='"+localAccount->getMachineId()+"'");
  //To get the database number id of the user
  std::string numUser = userServer.getAttribut("where userid='"+localAccount->getUserId()+"'");

  
  sqlInsert.append("('"+numMachine+"', '"+numUser+"', '"+localAccount->getAcLogin()+"', '\
		"+localAccount->getSshKeyPath()+"', '"+localAccount->getHomeDirectory()+"');");
  
  
  
}
