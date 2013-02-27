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
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include <string>
#include <vector>

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
ServerTMS::getBatchType() const {
  return mbatchType;
}

/**
 * \brief To get the batchVersion
 * \return the version of the underlying batch scheduler
 */
std::string
ServerTMS::getBatchVersion() const {
  return mbatchVersion;
}

/**
 * \brief To get the machine id of the TMS server
 * \return the machine id
 */
std::string
ServerTMS::getMachineId() const {
  return mmachineId;
}

/**
 * \brief To get the slave binary directory
 * \return path to the binary tmsSlave
 */
string
ServerTMS::getSlaveDirectory() const {
  return mslaveBinDir;
}

/**
 * \brief To get the main configuration
 * \return the pointer to configuration object
 */
ExecConfiguration_Ptr
ServerTMS::getSedConfig() const {
  return msedConfig;
}

/**
 * \brief Constructor (private)
 */
ServerTMS::ServerTMS(): mbatchType(UNDEFINED), mdatabaseVishnu(NULL) {}

/**
 * \brief To get the Default Batch Options
 * \return batch Default Options Vector
 */
std::vector<std::string>
ServerTMS::getDefaultBatchOption() const {
  return mdefaultBatchOption;
}

/**
 * \brief To initialize the TMS Server class
 * \param vishnuId The identifier of the vishnu instance
 * \param dbConfig  The configuration of the database
 * \param machineId the id of the machine
 * \param batchType The type of the batch scheduler
 * \param slaveBinDir  the directory that contains the slave binary
 * \param sedConfig A pointer to the SeD configuration
 * \return raises an exception on error
 */
int
ServerTMS::init(int& vishnuId,
                DbConfiguration& dbConfig,
                const std::string& machineId,
                BatchType batchType,
                const std::string& slaveBinDir,
                const ExecConfiguration_Ptr sedConfig) {

  setenv("VISHNU_DEBUG_LEVEL", vishnu::convertToString(mdebugLevel).c_str(), 0);

  //set the machineId
  mmachineId = machineId;

  //initialization of the slave directory
  mslaveBinDir = slaveBinDir;

  // initialize the SeD configuration object
  msedConfig = sedConfig;

  // set the debug level
  bool isSetDebugLevel = sedConfig->getConfigValue(vishnu::DEBUG_LEVEL, mdebugLevel);
  if (!isSetDebugLevel || mdebugLevel < 0) {
    mdebugLevel = 0;
  }

  //initialization of the batchType
  mbatchType = batchType;

  //initialization of the batchVersion
  if (mbatchType != DELTACLOUD) {
    msedConfig->getRequiredConfigValue<std::string>(vishnu::BATCHVERSION, mbatchVersion);
  } else {
    mbatchVersion = "";
  }

  std::string batchDefaultConfigFile;
  if(msedConfig->getConfigValue(vishnu::DEFAULTBATCHCONFIGFILE, batchDefaultConfigFile)) {
    switch(mbatchType) {
    case TORQUE:
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#PBS");
      break;
    case LOADLEVELER:
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "# @");
      break;
    case SLURM:
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#SBATCH");
      break;
    case LSF:
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#BSUB");
      break;
    case SGE:
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#$");
      break;
    case PBSPRO:
      getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#PBS");
      break;
    case DELTACLOUD:
      //No yet supported
      break;
    case POSIX:
      //No yet supported
      break;
    default:
      break;
    }
  }

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
      throw SystemException (ERRCODE_DBERR, "The vishnuid is unrecognized");
    }

  } catch (VishnuException& e) {
    std::clog << "[TMS][ERROR] " << e.what() << " \n";
    exit(0);
  }

  initMap(machineId);
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
                            std::vector<std::string>& defaultOptions, const char* batchKey) {

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
      if(boost::algorithm::starts_with(line, batchKey)) {
        line = line.substr(std::string(batchKey).size());
        boost::algorithm::trim_left(line);
        pos = line.find(" ");
        if(pos!=std::string::npos) {
          key = line.substr(0,pos);
          boost::algorithm::trim(key);
          defaultOptions.push_back(key);
          line = line.substr(pos);
          boost::algorithm::trim(line);
          while((pos = line.find(","))!=std::string::npos) {
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
  } catch (...) {

  }
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

void
ServerTMS::initMap(std::string mid) {
  int (*functionPtr)(diet_profile_t*);

  functionPtr = solveSubmitJob;
  mcb[string(SERVICES_TMS[JOBSUBMIT])+"@"+mid] = functionPtr;
  functionPtr = solveCancelJob;
  mcb[string(SERVICES_TMS[JOBCANCEL])+"@"+mid] = functionPtr;
  functionPtr = solveJobInfo;
  mcb[string(SERVICES_TMS[JOBINFO])+"@"+mid] = functionPtr;
  functionPtr = solveGetListOfJobs;
  mcb[string(SERVICES_TMS[GETLISTOFJOBS])+"@"+mid] = functionPtr;
  functionPtr = solveGetListOfJobsProgression;
  mcb[string(SERVICES_TMS[GETJOBSPROGRESSION])+"@"+mid] = functionPtr;
  functionPtr = solveListOfQueues;
  mcb[string(SERVICES_TMS[GETLISTOFQUEUES])+"@"+mid] = functionPtr;
  functionPtr = solveJobOutPutGetResult;
  mcb[string(SERVICES_TMS[JOBOUTPUTGETRESULT])+"@"+mid] = functionPtr;
  functionPtr = solveJobOutPutGetCompletedJobs;
  mcb[string(SERVICES_TMS[JOBOUTPUTGETCOMPLETEDJOBS])+"@"+mid] = functionPtr;
  // Remove ?
  functionPtr = solveGetListOfJobs;
  mcb[SERVICES_TMS[GETLISTOFJOBS_ALL]] = functionPtr;
  functionPtr = solveSubmitJob;
  mcb[SERVICES_TMS[JOBSUBMIT_AUTOM]] = functionPtr;
  functionPtr = solveAddWork;
  mcb[SERVICES_TMS[ADDWORK]] = functionPtr;
}
