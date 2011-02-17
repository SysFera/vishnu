/**
 * \file ServerUMS.cc
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#include <stdio.h>
#include <string.h>
#include "ServerUMS.hh"

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
* \fn void init(std::string vishnuid)
* \param vishnuid The id of the vishnu configuration registered in the database
*/
void
ServerUMS::init(std::string vishnuid, std::string password) {
  
  //TODO: le mot de passe du rôle pour la base de données il faudra le prendre en paramètre
  DbFactory factory;
  Database *mdatabaseVishnu = factory.getDatabaseInstance(POSTGREDB, "", "vishnu_user", password, "vishnu");
  
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
  UserServer admin = UserServer("vishnu_user", "vishnu_user");
  
    if (!admin.exist()) {
     
     mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid, pwd, privilege, passwordstate, status)\
     values ("+Vishnuid::mvishnuid+", 'vishnu_db_admin','ztV1aPn8GPzSLab1EL5kBaqyyJLrL3XXxSVgdl1.TPhghpkUXejxSLPntLq8keE/iJZXpXkGo848XzLYJMFvB.', 1, 1, 1)");
   
     mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid, pwd, privilege, passwordstate, status)\
     values ("+Vishnuid::mvishnuid+", 'vishnu_user','ztV1aPn8GPzSLab1EL5kBaqyyJLrL3XXxSVgdl1.TPhghpkUXejxSLPntLq8keE/iJZXpXkGo848XzLYJMFvB.', 1, 1, 1)");
    }
    else {
    std::cout << "The default users are already defined in the database"<< std::endl;	
    }
      
  } catch (SystemException& e) {
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
  //if (diet_service_table_add(profile, NULL, solveSessionConnect)) return 1;TODO throw exception
  diet_service_table_add(mprofile, NULL, solveSessionConnect);
 
  
  /* solveSessionReconnect */
  
  mprofile = diet_profile_desc_alloc(SRV[1], 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR); 
  diet_generic_desc_set(diet_param_desc(mprofile,6),DIET_STRING, DIET_CHAR); 
  //if (diet_service_table_add(profile, NULL, solveSessionReconnect)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveSessionReconnect);
  
  /* solveSessionClose */
  
  mprofile = diet_profile_desc_alloc(SRV[2], 0, 0, 1);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveSessionClose);

  /* solveUserCreate */
  
  mprofile = diet_profile_desc_alloc(SRV[3], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveUserCreate);
  
  /* solveUserUpdate */
  
  mprofile = diet_profile_desc_alloc(SRV[4], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveUserUpdate);
  
  /* solveUserDelete */
  
  mprofile = diet_profile_desc_alloc(SRV[5], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveUserDelete);
  
  /* solveUserPasswordChange */
  
  mprofile = diet_profile_desc_alloc(SRV[6], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveUserPasswordChange);
 
  /* solveUserPasswordReset */
  
  mprofile = diet_profile_desc_alloc(SRV[7], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveUserPasswordReset);
  
  /* solveMachineCreate */
  
  mprofile = diet_profile_desc_alloc(SRV[8], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveMachineCreate);
  
  /* solveMachineUpdate */
  
  mprofile = diet_profile_desc_alloc(SRV[9], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveMachineUpdate);
  
  /* solveMachineDelete */
  
  mprofile = diet_profile_desc_alloc(SRV[10], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveMachineDelete);
  
  /* solveLocalAccountCreate */
  
  mprofile = diet_profile_desc_alloc(SRV[11], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveLocalAccountCreate);
  
  /* solveLocalAccountUpdate */
  
  mprofile = diet_profile_desc_alloc(SRV[12], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveLocalAccountUpdate);
  
  /* solveLocalAccountDelete */
  
  mprofile = diet_profile_desc_alloc(SRV[13], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveLocalAccountDelete);
  
  /* solveOptionValueSet */
  
  mprofile = diet_profile_desc_alloc(SRV[16], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveOptionValueSet);
  
  /* solveOptionValueSetDefault */
  
  mprofile = diet_profile_desc_alloc(SRV[17], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveOptionValueSetDefault);
  
  
  /* solveRestore */
  
  mprofile = diet_profile_desc_alloc(SRV[24], 1, 1, 1);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(mprofile, NULL, solveRestore);
  

  diet_profile_desc_free(mprofile);
}

/**
 * \fn ~ServerUMS()
 * \brief Destructor, raises an exception on error
 */
ServerUMS::~ServerUMS() {  
}

