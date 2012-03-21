/**
* \file ServerTMS.cpp
* \brief This file presents the implementation of the TMS server.
* \author Daouda Traore (daouda.traore@sysfera.com)
* \date April
*/

#include "ServerTMS.hpp"
#include "internalApi.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include <boost/scoped_ptr.hpp>
#include "SystemException.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"
#include "UserServer.hpp"

//{{RELAX<MISRA_0_1_3> Because these variables are used this class
ServerTMS *ServerTMS::minstance = NULL;
TMSMapper *ServerTMS::mmapper = NULL;
//}}RELAX<MISRA_0_1_3>

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
 * \brief To get the unique instance of the database
 */
Database*
ServerTMS::getDatabaseVishnu() {
  return mdatabaseVishnu;
}

/**
* \brief To get the vishnuId
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
 * \brief To get the slave binary directory
 * \return path to the binary tmsSlave
 */
string
ServerTMS::getSlaveDirectory() {
  return mslaveBinDir;
}

/**
* \brief Constructor (private)
*/
ServerTMS::ServerTMS() {
  mprofile = NULL;
  mbatchType = UNDEFINED;
  mmachineId = "";
  mdatabaseVishnu = NULL;
  mslaveBinDir = "";
}

/**
 * \brief To initialize the TMS Server class
 * \param vishnuId The identifier of the vishnu instance
 * \param dbConfig  The configuration of the database
 * \param machineId the id of the machine
 * \param batchType the type of batch scheduler
 * \param slaveBinDir  the directory that contains the slave binary
 * \return raises an exception on error
 */
int
ServerTMS::init(int vishnuId,
                DbConfiguration dbConfig,
                std::string machineId,
                BatchType batchType,
                std::string slaveBinDir
               )
{

  //initialization of the batchType
  mbatchType = batchType;

  //initialization of the machineId
  mmachineId = machineId;

  //initialization of the slave directory
  mslaveBinDir = slaveBinDir;

  // initialization of the service table
  diet_service_table_init(NB_SRV);

  DbFactory factory;

   try {
    mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);

    //initialization of vishnuId
    mvishnuId = vishnuId;

    /*connection to the database*/
    mdatabaseVishnu->connect();

    mmapper = new TMSMapper(MapperRegistry::getInstance(), vishnu::TMSMAPPERNAME);
    mmapper->registerMapper();

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
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,6), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveSubmitJob)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* cancelJob */
  mprofile = diet_profile_desc_alloc((SERVICES[1]+std::string(machineId)).c_str(), 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveCancelJob)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* JobInfo */
  mprofile = diet_profile_desc_alloc((SERVICES[2]+std::string(machineId)).c_str(), 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveJobInfo)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* ListOfJobs */
  mprofile = diet_profile_desc_alloc((SERVICES[3]+std::string(machineId)).c_str(), 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveGetListOfJobs)) {
     return 1;
  }
  diet_profile_desc_free(mprofile);

  /* ListOfJobsProgression */
  mprofile = diet_profile_desc_alloc((SERVICES[4]+std::string(machineId)).c_str(), 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveGetListOfJobsProgression)) {
     return 1;
  }
  diet_profile_desc_free(mprofile);

  /* listOfQueues */
  mprofile = diet_profile_desc_alloc((SERVICES[5]+std::string(machineId)).c_str(), 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveListOfQueues)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

   /* jobOutPutGetResult */
  mprofile = diet_profile_desc_alloc((SERVICES[6]+std::string(machineId)).c_str(), 3, 3, 5);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_CONTAINER, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveJobOutPutGetResult)) { 
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* JobOutPutGetCompletedJobs */
  mprofile = diet_profile_desc_alloc((SERVICES[7]+std::string(machineId)).c_str(), 2, 2, 5);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_CONTAINER, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveJobOutPutGetCompletedJobs)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);
  /* ListOfJobs on all machines */
  mprofile = diet_profile_desc_alloc(SERVICES[8], 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveGetListOfJobs)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);
  /* automatic submitJob */
  mprofile = diet_profile_desc_alloc(SERVICES[9], 4, 4, 6);
  diet_aggregator_desc_t *agg;
  //TO SELECT a SeD
  {
    agg = diet_profile_desc_aggregator(mprofile);
    diet_service_use_perfmetric(ServerTMS::setBatchLoadPerformance);
    diet_aggregator_set_type(agg, DIET_AGG_PRIORITY);
    diet_aggregator_priority_minuser(agg, 0);
  }

  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,6), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveSubmitJob)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  return 0;
}

/**
 * \brief Function to compute the batch load performance (number of waiting jobs, running jobs and total jobs)
 * \param pb the resquest profile
 * \param perfValues The vector contain the estimation load performance (number of waiting jobs, running jobs and total jobs)
 */
void
ServerTMS::setBatchLoadPerformance(diet_profile_t* pb, estVector_t perfValues) {

  BatchFactory factory;
  BatchType batchType  = ServerTMS::getInstance()->getBatchType();
  
  boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchType));
  TMS_Data::ListJobs* listOfJobs = new TMS_Data::ListJobs();
  batchServer->fillListOfJobs(listOfJobs);

  char* sessionKey = (diet_paramstring_get_desc(diet_parameter(pb, 0)))->param;
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  long LoadValue = std::numeric_limits<long>::max();

  try {
    std::string machineId = ServerTMS::getInstance()->getMachineId();
    UserServer(sessionServer).getUserAccountLogin(machineId);
    char* jobSerialized = (diet_paramstring_get_desc(diet_parameter(pb, 3)))->param;
    TMS_Data::SubmitOptions_ptr submitOptions = NULL;
    if(vishnu::parseEmfObject(std::string(jobSerialized), submitOptions)) {
      if(submitOptions->getCriterion()!=NULL) {
        switch((submitOptions->getCriterion())->getLoadType()) {
          case 0 :
            LoadValue = listOfJobs->getNbWaitingJobs();
            break;
          case 1 :
            LoadValue = listOfJobs->getNbJobs();
            break;
          case 2 :
            LoadValue = listOfJobs->getNbRunningJobs();
            break;
          default :
            LoadValue = listOfJobs->getNbWaitingJobs();
            break;
        }
      } else {
        LoadValue = listOfJobs->getNbWaitingJobs();
      }
    }
  } catch (VishnuException& e) {
  }

  /*
   ** store the LoadValue value in the user estimate space,
   */
  diet_est_set(perfValues, 0, LoadValue);

  delete listOfJobs;
}

/**
* \brief Destructor, raises an exception on error
*/
ServerTMS::~ServerTMS() {
  if (mmapper != NULL) {
    delete mmapper;
  }
  if (mdatabaseVishnu != NULL) {
    delete mdatabaseVishnu;
  }
}
