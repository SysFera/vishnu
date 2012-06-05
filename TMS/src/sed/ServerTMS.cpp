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
#include <boost/algorithm/string.hpp>
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
//  mprofile = NULL;
  mbatchType = UNDEFINED;
  mmachineId = "";
  mdatabaseVishnu = NULL;
  mslaveBinDir = "";
}
/**
 * \brief To get the Default Batch Options
 * \return batch Default Options Vector
 */
std::vector<std::string>
ServerTMS::getDefaultBatchOption(){
  return mdefaultBatchOption;
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
                std::string slaveBinDir,
                std::string batchDefaultConfigFile
               )
{

  //initialization of the batchType
  mbatchType = batchType;

  //initialization of the machineId
  mmachineId = machineId;

  //initialization of the slave directory
  mslaveBinDir = slaveBinDir;

  //initialization of the default batch config file
 // mdefaultBatchConfig = batchDefaultConfigFile;
  switch(mbatchType) {
    case TORQUE :
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#PBS");
      break;              
    case LOADLEVELER :
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "# @");
      break;
    case SLURM :
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#SBATCH");
      break;
    case LSF :
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#BSUB");
      break;
    case SGE :
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#$");
      break;
    case PBSPRO :
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#PBS");
      break;
    default :
      break;
  }

  // initialization of the service table
//  diet_service_table_init(NB_SRV);

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


  initMap(machineId);


//
//  // initialization of the service table
//  diet_service_table_init(NB_SRV);
//  /* submitJob */
//  mprofile = diet_profile_desc_alloc((SERVICES[0]+std::string(machineId)).c_str(), 4, 4, 6);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,6), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveSubmitJob)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* cancelJob */
//  mprofile = diet_profile_desc_alloc((SERVICES[1]+std::string(machineId)).c_str(), 2, 2, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveCancelJob)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* JobInfo */
//  mprofile = diet_profile_desc_alloc((SERVICES[2]+std::string(machineId)).c_str(), 2, 2, 4);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveJobInfo)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* ListOfJobs */
//  mprofile = diet_profile_desc_alloc((SERVICES[3]+std::string(machineId)).c_str(), 2, 2, 4);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveGetListOfJobs)) {
//     return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* ListOfJobsProgression */
//  mprofile = diet_profile_desc_alloc((SERVICES[4]+std::string(machineId)).c_str(), 2, 2, 4);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveGetListOfJobsProgression)) {
//     return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* listOfQueues */
//  mprofile = diet_profile_desc_alloc((SERVICES[5]+std::string(machineId)).c_str(), 2, 2, 4);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveListOfQueues)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//   /* jobOutPutGetResult */
//  mprofile = diet_profile_desc_alloc((SERVICES[6]+std::string(machineId)).c_str(), 3, 3, 5);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_CONTAINER, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveJobOutPutGetResult)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* JobOutPutGetCompletedJobs */
//  mprofile = diet_profile_desc_alloc((SERVICES[7]+std::string(machineId)).c_str(), 2, 2, 5);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_CONTAINER, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveJobOutPutGetCompletedJobs)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//  /* ListOfJobs on all machines */
//  mprofile = diet_profile_desc_alloc(SERVICES[8], 2, 2, 4);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveGetListOfJobs)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//  /* automatic submitJob */
//  mprofile = diet_profile_desc_alloc(SERVICES[9], 4, 4, 6);
//  diet_aggregator_desc_t *agg;
//  //TO SELECT a SeD
//  {
//    agg = diet_profile_desc_aggregator(mprofile);
//    diet_service_use_perfmetric(ServerTMS::setBatchLoadPerformance);
//    diet_aggregator_set_type(agg, DIET_AGG_PRIORITY);
//    diet_aggregator_priority_minuser(agg, 0);
//  }
//
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,5), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,6), DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveSubmitJob)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);

  return 0;
}


/**
 * \brief Function to get the default Batch submission options
 * \param configPath The job script path
 * \param defaultOptions The list of the option value
 * \return raises an exception on error
 */

void
ServerTMS::getConfigOptions(const char* configPath,
                            std::vector<std::string>& defaultOptions, const char* batchKey){

 try {
    std::string scriptContent = vishnu::get_file_content(configPath);
    std::istringstream iss(scriptContent);
    std::string line;
    std::string value;
    std::string key;
    while(!iss.eof()) {
      getline(iss, line);
      size_t pos = line.find('#');
      if(pos==string::npos) {
        continue;
      }
     
      line = line.erase(0, pos);
      if(boost::algorithm::starts_with(line, batchKey)){
        line = line.substr(std::string(batchKey).size());
        boost::algorithm::trim_left(line);
        pos = line.find(" ");
        if(pos!=std::string::npos){
          key = line.substr(0,pos);
          boost::algorithm::trim(key);
          defaultOptions.push_back(key);
          line = line.substr(pos);
          boost::algorithm::trim(line);
          while((pos = line.find(","))!=std::string::npos){
            value = line.substr(0,pos-1);
            defaultOptions.push_back(value);
            defaultOptions.push_back(key);
            line = line.erase(0,pos);
          }
          value = line;
          defaultOptions.push_back(value);
        }
      }
    }
  } catch (...){
   
  }  
}


//void
//ServerTMS::setBatchLoadPerformance(diet_profile_t* pb, estVector_t perfValues) {
//
//  BatchFactory factory;
//  BatchType batchType  = ServerTMS::getInstance()->getBatchType();
//
//  boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchType));
//  TMS_Data::ListJobs* listOfJobs = new TMS_Data::ListJobs();
//  batchServer->fillListOfJobs(listOfJobs);
//
//  char* sessionKey = (diet_paramstring_get_desc(diet_parameter(pb, 0)))->param;
//  SessionServer sessionServer = SessionServer(std::string(sessionKey));
//  long LoadValue = std::numeric_limits<long>::max();
//
//  try {
//    std::string machineId = ServerTMS::getInstance()->getMachineId();
//    UserServer(sessionServer).getUserAccountLogin(machineId);
//    char* jobSerialized = (diet_paramstring_get_desc(diet_parameter(pb, 3)))->param;
//    TMS_Data::SubmitOptions_ptr submitOptions = NULL;
//    if(vishnu::parseEmfObject(std::string(jobSerialized), submitOptions)) {
//      if(submitOptions->getCriterion()!=NULL) {
//        switch((submitOptions->getCriterion())->getLoadType()) {
//          case 0 :
//            LoadValue = listOfJobs->getNbWaitingJobs();
//            break;
//          case 1 :
//            LoadValue = listOfJobs->getNbJobs();
//            break;
//          case 2 :
//            LoadValue = listOfJobs->getNbRunningJobs();
//            break;
//          default :
//            LoadValue = listOfJobs->getNbWaitingJobs();
//            break;
//        }
//      } else {
//        LoadValue = listOfJobs->getNbWaitingJobs();
//      }
//    }
//  } catch (VishnuException& e) {
//  }
//
//  /*
//   ** store the LoadValue value in the user estimate space,
//   */
//  diet_est_set(perfValues, 0, LoadValue);
//
//  delete listOfJobs;
//}

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

void
ServerTMS::initMap(std::string mid) {
  int (*functionPtr)(diet_profile_t*);

  functionPtr = solveSubmitJob;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[0])+mid,functionPtr));
  functionPtr = solveCancelJob;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[1])+mid,functionPtr));
  functionPtr = solveJobInfo;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[2])+mid,functionPtr));
  functionPtr = solveGetListOfJobs;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[3])+mid,functionPtr));
  functionPtr = solveGetListOfJobsProgression;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[4])+mid,functionPtr));
  functionPtr = solveListOfQueues;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[5])+mid,functionPtr));
  functionPtr = solveJobOutPutGetResult;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[6])+mid,functionPtr));
  functionPtr = solveJobOutPutGetCompletedJobs;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[7])+mid,functionPtr));
// Remove ?
  functionPtr = solveGetListOfJobs;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[8]),functionPtr));
  functionPtr = solveSubmitJob;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[9]),functionPtr));
  functionPtr = solveAddWork;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[10]),functionPtr));
}

int
ServerTMS::call(diet_profile_t* profile){
  map<string, functionPtr_t>::iterator it;
  it = mcb.find(string(profile->name));
  if (it == mcb.end()) {
    std::cerr << "not found service : " << profile->name << std::endl;
    return 0;
  }
  int (*functionPtr)(diet_profile_t*);
  functionPtr = it->second;
  return (*functionPtr)(profile);
}
