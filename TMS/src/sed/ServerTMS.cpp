/**
* \file ServerTMS.cpp
* \brief This file presents the implementation of the TMS server.
* \author Daouda Traore (daouda.traore@sysfera.com)
* \date April
*/

#include "ServerTMS.hpp"
#include "internalTMSAPI.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include <boost/scoped_ptr.hpp>
#include "SystemException.hpp"

ServerTMS *ServerTMS::minstance = NULL;
BatchType ServerTMS::mbatchType = UNDEFINED;
std::string ServerTMS::mmachineId = "";
Database *ServerTMS::mdatabaseVishnu = NULL;

/**
 * \brief To get the unique instance of the server
 */
ServerTMS*
ServerTMS::getInstance() {
  if (minstance == NULL) {
    minstance = new ServerTMS();
  }
  return minstance;
}

/**
* \brief To get the vishnuId
* \fn int getVishnuId()
* \return the path of the configuration file
*/
int
ServerTMS::getVishnuId() const {
  return mvishnuId;
}

/**
  * \brief To get the batchType
  * \return the id of the underlying batch scheduler
  */
BatchType
ServerTMS::getBatchType()  {
  return mbatchType;
}

/**
  * \brief To get the machine id of the TMS server
  * \return the machine id
  */
std::string
ServerTMS::getMachineId() {
  return mmachineId;
}

/**
* \brief Constructor (private)
* \fn ServerTMS()
*/
ServerTMS::ServerTMS() : mprofile(NULL) {
}

/**
 * \brief To initialize the TMS Server class
 * \param vishnuId The identifier of the vishnu instance
 * \param dbConfig  The configuration of the database
 * \param machineId the id of the machine
 * \param batchType the type of batch scheduler
 */
int
ServerTMS::init(int vishnuId, DbConfiguration dbConfig, std::string machineId, BatchType batchType)
{

  //initialization of the batchType
  mbatchType = batchType;

  //initialization of the machineId
  mmachineId = machineId;

  // initialization of the service table
  diet_service_table_init(NB_SRV);


  DbFactory factory;

   try {
    mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
 
    //initialization of vishnuId
    mvishnuId = vishnuId;
  
    /*connection to the database*/
    mdatabaseVishnu->connect();

    std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+vishnu::convertToString(vishnuId));

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));

    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;
    }


  } catch (VishnuException& e) {
      std::cout << e.what() << std::endl;
      exit(0);
  }


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
* \fn ~ServerTMS()
* \brief Destructor, raises an exception on error
*/
ServerTMS::~ServerTMS() {

}
