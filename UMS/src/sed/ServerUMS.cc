/**
 * \file ServerUMS.cc
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#include "ServerUMS.hh"

/**
 * \brief Function to solve the service SessionConnect 
 * \fn    void solveSessionConnect(diet_profile_t* pb);
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionConnect(diet_profile_t* pb) {  
}
/**
 * \brief Function to solve the service SessionReconnect 
 * \fn    void solveSessionReconnect
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionReconnect(diet_profile_t* pb){}
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

  //TODO Vishnu_initialize et Vishnu_Finalize
  vishnucfg = cfg;
  profile = NULL;
 
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
  //if (diet_service_table_add(profile, NULL, solveSessionReconnect)) return 1;
  diet_service_table_add(profile, NULL, solveSessionReconnect);
  
  /* solveSessionClose */
  
  profile = diet_profile_desc_alloc(SRV[2], 0, 0, 1);
  diet_generic_desc_set(diet_param_desc(profile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1),DIET_STRING, DIET_CHAR);
  //if (diet_service_table_add(profile, NULL, solveSessionClose)) return 1;
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

