/**
* \file TMSServer.cpp
* \brief This file presents the implementation of the TMS server.
* \author Daouda Traore (daouda.traore@sysfera.com)
* \date April 
*/

#include "TMSServer.hpp"
#include "internalTMSAPI.hpp"

TMSServer *TMSServer::minstance = NULL;
BatchType TMSServer::mbatchType = UNDEFINED;
std::string TMSServer::mmachineId = "";

/**
 * \brief To get the unique instance of the server
 */
TMSServer*
TMSServer::getInstance() {
  if (minstance == NULL) {
    minstance = new TMSServer();
  }
  return minstance;
}

/**
  * \brief To get the batchType
  * \return the id of the underlying batch scheduler 
  */
BatchType
TMSServer::getBatchType()  {
  return mbatchType;
}

/**
  * \brief To get the machine id of the TMS server 
  * \return the machine id  
  */
std::string
TMSServer::getMachineId() {
  return mmachineId;
}

/**
* \brief Constructor (private)
* \fn TMSServer()
*/
TMSServer::TMSServer() : mprofile(NULL) {
}

/**
 * \brief To initialize the TMS Server class
 * \param bachType the type of batch scheduler 
 * \param machineId the id of the machine 
 */
int
TMSServer::init(BatchType batchType, std::string machineId)
{

  //initialization of the batchType
  mbatchType = batchType;

  //initialization of the machineId
  mmachineId = machineId;

  // initialization of the service table
  diet_service_table_init(NB_SRV);


  // initialization of the service table
  diet_service_table_init(NB_SRV);

  /* submitJob */
  mprofile = diet_profile_desc_alloc((SERVICES[0]+std::string(machineId)).c_str(), 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,6), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solve_submitJob)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* cancelJob */
  mprofile = diet_profile_desc_alloc((SERVICES[1]+std::string(machineId)).c_str(), 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solve_cancelJob)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  return 0;
}

/**
* \fn ~TMSServer()
* \brief Destructor, raises an exception on error
*/
TMSServer::~TMSServer() {

}
