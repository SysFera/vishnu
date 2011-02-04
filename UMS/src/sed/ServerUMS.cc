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
  return vishnucfg;
}
/**
* \brief Constructor, raises an exception on error
* \fn ServerUMS(std::string cfg)
* \param cfg The vishnu configuration filepath
*/  
ServerUMS::ServerUMS(std::string cfg) {
  //TODO Vishnu_initialize et Vishnu_Finalize
  vishnucfg = cfg;
  profile = NULL;
}


/**
* \brief To initialize the UMS server
* \fn void init()
*/
void
ServerUMS::init() {

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
}

/**
 * \fn ~ServerUMS()
 * \brief Destructor, raises an exception on error
 */
ServerUMS::~ServerUMS() {  
}

