/**
 * \file ServerUMS.cc
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#include <stdio.h>
#include <string.h>

#include "ServerUMS.hh"
#include "UserServer.hh"
#include "SessionServer.hh"
#include "MachineClientServer.hh"

#include "POSTGREDatabase.hh"
#include "DbFactory.hh"
#include "DatabaseResult.hh"

#include <exception>
#include "SystemException.hh"




/**
 * \brief Function to solve the service SessionConnect 
 * \fn    void solveSessionConnect(diet_profile_t* pb);
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionConnect(diet_profile_t* pb) {  

  char* userId = NULL;
  char* password = NULL;
  char* clientKey = NULL;
  char* clientHostname = NULL;
  char* options = NULL;
  //char* errorInfo = NULL;
  
    
  diet_string_get(diet_parameter(pb,0), &userId, NULL);
  std::cout<<"userID:"<<userId<<std::endl;
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  std::cout<<"password:"<<password<<std::endl;
  diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
  std::cout<<"clientKey:"<<clientKey<<std::endl;
  diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
  std::cout<<"clientHostname:"<<clientHostname<<std::endl;
  diet_string_get(diet_parameter(pb,4), &options, NULL);
  
  UserServer userServer = UserServer(std::string(userId), std::string(password));
  MachineClientServer machineClientServer =  MachineClientServer(std::string(clientKey), std::string(clientHostname));
  SessionServer sessionServer("");
          
     ecorecpp::parser::parser parser;
     
     //TODO :  à décommenter pour les tests
     ConnectOptions_ptr connectOpt;// = parser.load(std::string(options))->as< ConnectOptions >();
     //ListSessions_ptr other_listssession = parser.load(options)->as< ConnectOptions >();
     try { 
     std::string empty("");  
     sessionServer.connectSession(userServer, machineClientServer, connectOpt);
     diet_string_set(diet_parameter(pb,5), strdup(sessionServer.getData().getSessionKey().c_str()), DIET_VOLATILE);
     diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
  
     } catch (SystemException& e) {
	std::string empty("");
	std::string errorInfo = e.getMsg()+"==>";
	errorInfo.append(e.what());
	
	std::cout << "errorInfo: " << errorInfo <<std::endl;
	
	diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
	diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
  
  }
   
  
}
/**
 * \brief Function to solve the service SessionReconnect 
 * \fn    void solveSessionReconnect
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionReconnect(diet_profile_t* pb){
  
  char* userId = NULL;
  char* password = NULL;
  char* clientKey = NULL;
  char* clientHostname = NULL;
  char* sessionId = NULL;
  //char* errorInfo = NULL;
  
  diet_string_get(diet_parameter(pb,0), &userId, NULL);
  std::cout<<"userID:"<<userId<<std::endl;
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  std::cout<<"password:"<<password<<std::endl;
  diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
  std::cout<<"clientKey:"<<clientKey<<std::endl;
  diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
  std::cout<<"clientHostname:"<<clientHostname<<std::endl;
  diet_string_get(diet_parameter(pb,4), &sessionId, NULL);
  
  UserServer userServer = UserServer(std::string(userId), std::string(password));
  
  //If the user is on the database
  if (userServer.exist()) {
     
    
     MachineClientServer machineClientServer =  MachineClientServer(std::string(clientKey), std::string(clientHostname));
     SessionServer sessionServer("");
     sessionServer.getData().setSessionId(std::string("sessionId"));
     sessionServer.reconnect();
     
     //Genration of sessionKey and sessionId which are put directly on the UMS_Data::Session of sessionServer
     //sessionServer.generateSessionKey();
     //sessionServer.generateSessionId();
     
     //ecorecpp::parser::parser parser;
     
     //TODO :  à décommenter pour les tests
     //ConnectOptions_ptr connectOpt;// = parser.load(std::string(options))->as< ConnectOptions >();
     //ListSessions_ptr other_listssession = parser.load(options)->as< ConnectOptions >();
      
     //sessionServer.connectSession(userServer, machineClientServer, connectOpt);
     
     /*diet_string_set(diet_parameter(pb,5), sessionServer.getData.getSessionKey(), DIET_VOLATILE); 
     diet_string_set(diet_parameter(pb,6), errorInfo, DIET_VOLATILE);*/
    
     
	std::string empty("");
	diet_string_set(diet_parameter(pb,5), strdup(sessionServer.getData().getSessionKey().c_str()), DIET_VOLATILE);
	diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
  
     
  } // End If the user is on the database
  else {
	std::string empty("");
	std::string errorInfo("The user is unknown");
	diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
	diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
      
  }
  
  
  
  
  
  
  
  
  
  
  
}
/**
 * \brief Function to solve the service SessionClose 
 * \fn    virtual int createDatabase() = 0
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionClose(diet_profile_t* pb){}

/**
* \brief To get the path of the configuration file used by the UMS server
* \fn std::string getCfg()
* \return the path of the configuration file
*/
std::string
ServerUMS::getCfg() {
  return vishnucfg;
}
/**
* \brief Constructor, raises an exception on error
* \fn ServerUMS(std::string cfg)
* \param cfg The vishnu configuration filepath
*/  
ServerUMS::ServerUMS(std::string cfg) {

  //DbFactory factory;
  //mdatabaseVishnu = factory.create(POSTGREDB, "", "", "", "VISHNU");
  
  //TODO Vishnu_initialize et Vishnu_Finalize
  vishnucfg = cfg;
  profile = NULL;
 
  //TODO: Mettre tout ce qui suit dans une fonction initialise
  DbFactory factory;
  Database *mdatabaseVishnu = factory.getDatabaseInstance(POSTGREDB, "", "", "", "VISHNU");
  
  try {
  //connection of the database
  mdatabaseVishnu->connect();
  
  //Insertion of the default vishnu user
  UserServer admin = UserServer("admin", "admin");
  
    if (!admin.exist()) {
     std::cout << "Insertion of the default Vishnu user";
     mdatabaseVishnu->process("insert into vishnu (updatefreq, usercpt) values (0, 0)");    
     mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid,pwd) values (1, 'admin','admin')");
    }
    else {
    std::cout << "The default user is already defined in the database"<< std::endl;	
    }
    
    
  
  } catch (SystemException& e) {
	std::cout << "Message generique <-> 1: " << e.getMsg()<<std::endl;
	std::cout << "Details supplementaires 2: " << e.what() <<std::endl;  
  }
  
  
  // initialization of the service table
  diet_service_table_init(NB_SRV);  
  
  /* solveSessionConnect */
  
  profile = diet_profile_desc_alloc(SRV[0], 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(profile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,5),DIET_STRING, DIET_CHAR); 
  diet_generic_desc_set(diet_param_desc(profile,6),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionConnect)) return 1;TODO throw exception
  diet_service_table_add(profile, NULL, solveSessionConnect);
 
  
  /* solveSessionReconnect */
  
  profile = diet_profile_desc_alloc(SRV[1], 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(profile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,5),DIET_STRING, DIET_CHAR); 
  diet_generic_desc_set(diet_param_desc(profile,6),DIET_STRING, DIET_CHAR); 
  //if (diet_service_table_add(profile, NULL, solveSessionReconnect)) return 1; TODO throw exception
  diet_service_table_add(profile, NULL, solveSessionReconnect);
  
  /* solveSessionClose */
  
  profile = diet_profile_desc_alloc(SRV[2], 0, 0, 1);
  diet_generic_desc_set(diet_param_desc(profile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(profile, NULL, solveSessionClose);

  diet_profile_desc_free(profile);
  
  //diet_print_service_table();
  //res = diet_SeD(argv[1], argc, argv);
  
}

//ServerUMS(std::string cfg);
/**
 * \fn ~ServerUMS()
 * \brief Destructor, raises an exception on error
 */
ServerUMS::~ServerUMS() {  
}

