
#include "ConfigurationServer.hpp"


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

std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users";

std::string sqlListofMachines = "SELECT machineid, name, site, status, lang, description from machine, description \
where machine.nummachineid = description.machine_nummachineid";

std::string sqlListofLocalAccount = "SELECT * from account";

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
		user->setStatus(convertToInt(*(ii)));
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
 
int ConfigurationServer::restore()
{
  return 0;
}
 
ConfigurationServer::~ConfigurationServer() {
}
 
UMS_Data::Configuration*
ConfigurationServer::getConfiguration()
{
  return mconfiguration;
}
