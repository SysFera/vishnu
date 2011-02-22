/**
 * \file ServerUMS.cc
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#include <stdio.h>
#include <string.h>
#include "ServerUMS.hpp"

/**
* \brief To get the path of the configuration file used by the UMS server
* \fn std::string getCfg()
* \return the path of the configuration file
*/
std::string
ServerUMS::getCfg() {
  return mvishnucfg;
}
/**
* \brief Constructor, raises an exception on error
* \fn ServerUMS(std::string cfg)
* \param cfg The vishnu configuration filepath
* \param vishnuid The id of the vishnu configuration registered in the database which will be used
*/  
ServerUMS::ServerUMS(std::string cfg) {
  mvishnucfg = cfg;
  mprofile = NULL;
  //mvishnuid = vishnuid;
  //Vishnuid::mvishnuid = vishnuid; 
}

/**
* \brief To initialize the UMS server
* \fn int init(std::string vishnuid)
* \param vishnuid The id of the vishnu configuration registered in the database
* \return an error code (0 if success and 1 if an error occurs) 
*/
int
ServerUMS::init(std::string vishnuid, std::string password) {
  
  DbFactory factory;
  
  //TODO: prendre en paramètre le type de base de données
  
  //To get database instance
  Database *mdatabaseVishnu = factory.getDatabaseInstance(POSTGREDB, "", 
							  "vishnu_user", password, "vishnu");
  Vishnuid::mvishnuid = vishnuid;
  DatabaseResult *result;
  
  std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+Vishnuid::mvishnuid);
  std::cout <<"SQL COMMAND:"<<sqlCommand;
  
  try {
  
  /*connection to the database*/
  mdatabaseVishnu->connect();
  
  /* Checking of vishnuid on the database */
  result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 
  if (result->getResults().size() == 0) {
   SystemException e(4, "The vishnuid is unrecognized"); 
   throw e;
  }  
  
  //The default vishnu users
  UserServer admin = UserServer("vishnu_user",
  "ztV1aPn8GPzSLab1EL5kBaqyyJLrL3XXxSVgdl1.TPhghpkUXejxSLPntLq8keE/iJZXpXkGo848XzLYJMFvB.");
  
    if (!admin.exist()) {
     
     mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid, pwd, privilege, passwordstate, status)\
     values ("+Vishnuid::mvishnuid+", 'vishnu_db_admin','ztV1aPn8GPzSLab1EL5kBaqyyJLrL3XXxSVgdl1.TPhghpkUXejxSLPntLq8keE/iJZXpXkGo848XzLYJMFvB.', 1, 1, 1)");
   
     mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid, pwd, privilege, passwordstate, status)\
     values ("+Vishnuid::mvishnuid+", 'vishnu_user','ztV1aPn8GPzSLab1EL5kBaqyyJLrL3XXxSVgdl1.TPhghpkUXejxSLPntLq8keE/iJZXpXkGo848XzLYJMFvB.', 1, 1, 1)");
    }
    else {
    std::cout << "The default users are already defined in the database"<< std::endl;	
    }
      
  } catch (VishnuException& e) {
	std::cout << "Message generique <-> 1: " << e.getMsg()<<std::endl;
	std::cout << "Details supplementaires 2: " << e.what() <<std::endl;
	exit(0);
  }
  
  // initialization of the service table
  diet_service_table_init(NB_SRV);  
  
  /* solveSessionConnect */
  
  mprofile = diet_profile_desc_alloc(SRV[0], 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR); 
  diet_generic_desc_set(diet_param_desc(mprofile,6),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveSessionConnect)) return 1;
 
  
  /* solveSessionReconnect */
  
  mprofile = diet_profile_desc_alloc(SRV[1], 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR); 
  diet_generic_desc_set(diet_param_desc(mprofile,6),DIET_STRING, DIET_CHAR); 
  if (diet_service_table_add(mprofile, NULL, solveSessionReconnect)) return 1;
  
  /* solveSessionClose */
  
  mprofile = diet_profile_desc_alloc(SRV[2], 0, 0, 1);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveSessionClose)) return 1;
 
  /* solveUserCreate */
  
  mprofile = diet_profile_desc_alloc(SRV[3], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveUserCreate)) return 1;
  
  /* solveUserUpdate */
  
  mprofile = diet_profile_desc_alloc(SRV[4], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveUserUpdate)) return 1;
  
  /* solveUserDelete */
  
  mprofile = diet_profile_desc_alloc(SRV[5], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveUserDelete)) return 1;
  
  /* solveUserPasswordChange */
  
  mprofile = diet_profile_desc_alloc(SRV[6], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveUserPasswordChange)) return 1;
 
  /* solveUserPasswordReset */
  
  mprofile = diet_profile_desc_alloc(SRV[7], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveUserPasswordReset)) return 1;
  
  /* solveMachineCreate */
  
  mprofile = diet_profile_desc_alloc(SRV[8], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveMachineCreate)) return 1; 
  
  /* solveMachineUpdate */
  
  mprofile = diet_profile_desc_alloc(SRV[9], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveMachineUpdate)) return 1; 
  
  /* solveMachineDelete */
  
  mprofile = diet_profile_desc_alloc(SRV[10], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveMachineDelete)) return 1; 
  
  /* solveLocalAccountCreate */
  
  mprofile = diet_profile_desc_alloc(SRV[11], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveLocalAccountCreate)) return 1; 
  
  /* solveLocalAccountUpdate */
  
  mprofile = diet_profile_desc_alloc(SRV[12], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveLocalAccountUpdate)) return 1; 
  
  /* solveLocalAccountDelete */
  
  mprofile = diet_profile_desc_alloc(SRV[13], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveLocalAccountDelete)) return 1;
  
  /* solveConfigurationSave */
  
  mprofile = diet_profile_desc_alloc(SRV[14], 0, 0, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveConfigurationSave)) return 1; 
  
  /* solveConfigurationRestore */
  
  mprofile = diet_profile_desc_alloc(SRV[15], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveConfigurationRestore)) return 1; 
  
  
  /* solveOptionValueSet */
  
  mprofile = diet_profile_desc_alloc(SRV[16], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveOptionValueSet)) return 1; 
  
  /* solveOptionValueSetDefault */
  
  mprofile = diet_profile_desc_alloc(SRV[17], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveOptionValueSetDefault)) return 1; 
  
  /* solveListSessions */
  
  mprofile = diet_profile_desc_alloc(SRV[18], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveListSessions)) return 1;
  
  /* solveListLocalAccount */
   
  mprofile = diet_profile_desc_alloc(SRV[19], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveListLocalAccount)) return 1;
  
   /* solveListMachines */
  
  mprofile = diet_profile_desc_alloc(SRV[20], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveListMachines)) return 1;
  
   /* solveListHistoryCmd */
   
  mprofile = diet_profile_desc_alloc(SRV[21], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveListHistoryCmd)) return 1;
  
   /* solveListOptions */
   
  mprofile = diet_profile_desc_alloc(SRV[22], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveListOptions)) return 1; 
  
  /* solveListUsres */
  
  mprofile = diet_profile_desc_alloc(SRV[23], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveListUsers)) return 1;
  
  /* solveRestore */
  
  mprofile = diet_profile_desc_alloc(SRV[24], 0, 0, 0);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveRestore)) return 1; 
 
  diet_profile_desc_free(mprofile);
  
  return 0;
}

/**
 * \fn ~ServerUMS()
 * \brief Destructor, raises an exception on error
 */
ServerUMS::~ServerUMS() {  
}

