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
  //TODO: mettre mot de passe pour access à la base de données
  Database *mdatabaseVishnu = factory.getDatabaseInstance(POSTGREDB, "", "", "", "VISHNU");
  
  try {
  /*connection to the database*/
  mdatabaseVishnu->connect();
  
  //The default vishnu user
  UserServer admin = UserServer("admin", "admin");
  
    if (!admin.exist()) {
     std::cout << "Insertion of the default Vishnu user";
     
     //TODO: mettre toutes ces insertions dans le sql de création de la base de données
     /*Insertion of the vishnu object in the database en disant explicitement que les séquences commencent à 1*/
     mdatabaseVishnu->process("insert into vishnu (updatefreq, usercpt) values (0, 0)");
     
     /*Insertion of the default admin user*/
     mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid, pwd, privilege, passwordstate, deleted, blocked)\
     values (1, 'admin','admin', 1, 1, 0, 0)");
    //TODO: Récupérer le vishnu_vishnuid c'est plus propre (par défault ici c'est 1) ou éviter un incrément dans la table
    //vishnu et mettre vishnu_vishnuid à 1 idem pour ce qui suit
     
     /*Insertion of the default option values*/ //TODO optionid ne sert à rien l'autoincréménte suffit
     mdatabaseVishnu->process("insert into optionu (optionid, description, defaultvalue) values (1, 'VISHNU_CLOSE_POLICY', 1)");
     mdatabaseVishnu->process("insert into optionu (optionid, description, defaultvalue) values (2, 'VISHNU_TIMEOUT', 3600)");
     
     /*Insertion of the admin options value*/
     //mdatabaseVishnu->process("insert into optionvalue (users_numuserid, optionu_numoptionid, value) values (1, 1, -1)");
     //mdatabaseVishnu->process("insert into optionvalue (users_numuserid, optionu_numoptionid, value) values (1, 2, -1)");
     /*TODO: Récupérer le users_numuserid et optionu_numoptionid c'est plus propre 
      (par défault ici l'autoincréménte commence à 1)
      -1 signifie que l'option n'est pas définie.
      */
    
     
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

  /* solveUserCreate */
  
  profile = diet_profile_desc_alloc(SRV[3], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(profile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1; TODO throw exception
  diet_service_table_add(profile, NULL, solveUserCreate);
 
  diet_profile_desc_free(profile);
}

/**
 * \fn ~ServerUMS()
 * \brief Destructor, raises an exception on error
 */
ServerUMS::~ServerUMS() {  
}

