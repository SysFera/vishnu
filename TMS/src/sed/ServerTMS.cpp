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
 * \brief Function to select a server for automatic selection
 * \param sessionKey The session key
 * \param criterion The selection criterion
 * \return the machine id
 */
std::string
ServerTMS::selectMachine(const string& sessionKey, const TMS_Data::LoadCriterion_ptr & criterion) {

	std::cout << muriNamerCfg << std::endl;
	vishnu::vishnuInitialize(const_cast<char*>(muriNamerCfg.c_str()), 0, NULL) ;
	UserServer userServer = UserServer(SessionServer(sessionKey));
	userServer.init();
	string userId = userServer.getData().getUserId();

	if( userId.size() == 0 ) {
		throw UMSVishnuException(ERRCODE_UNKNOWN_USER, "unable to assign a user to the session. May be the session is no longer valid.");
	}

	UMS_Data::ListMachines machines;
	UMS_Data::ListMachineOptions mopts;
	mopts.setListAllMachine(false);
	mopts.setUserId(userId);
	mopts.setMachineId("");
	vishnu::listMachines(sessionKey, machines, mopts) ;
	if(machines.getMachines().size() == 0) {
		throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE, "there is no machine assigned to the user "+userId);
	}
	return machines.getMachines().get(0)->getMachineId();

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
                const std::string machineId,
                BatchType batchType,
                const std::string& slaveBinDir,
                const std::string& batchDefaultConfigFile,
    			const std::string& uriDispatcher
               )
{

  //initialization of the batchType
  mbatchType = batchType;

  //initialization of the machineId
  mmachineId = machineId;

  //initialization of the slave directory
  mslaveBinDir = slaveBinDir;

  //set the name uri
  vishnu::saveInFile(muriDispatcherCfg, "retryTimeout=10\ndispatcher="+uriDispatcher) ;

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
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[0])+"@"+mid,functionPtr));
  functionPtr = solveCancelJob;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[1])+"@"+mid,functionPtr));
  functionPtr = solveJobInfo;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[2])+"@"+mid,functionPtr));
  functionPtr = solveGetListOfJobs;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[3])+"@"+mid,functionPtr));
  functionPtr = solveGetListOfJobsProgression;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[4])+"@"+mid,functionPtr));
  functionPtr = solveListOfQueues;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[5])+"@"+mid,functionPtr));
  functionPtr = solveJobOutPutGetResult;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[6])+"@"+mid,functionPtr));
  functionPtr = solveJobOutPutGetCompletedJobs;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[7])+"@"+mid,functionPtr));
  // Remove ?
  functionPtr = solveGetListOfJobs;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[8]),functionPtr));
  functionPtr = solveSubmitJob;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[9]),functionPtr));
  functionPtr = solveAddWork;
  mcb.insert( pair<string, functionPtr_t> (string(SERVICES[10]),functionPtr));
}


/**
 * \brief Function to select a server for automatic selection
 * \param sessionKey The session key
 * \param criterion The selection criterion
 * \return the machine id
 */
std::string
ServerTMS::selectMachine(const string& sessionKey, const TMS_Data::LoadCriterion_ptr & criterion) {

	vishnu::vishnuInitialize(const_cast<char*>(muriNamerCfg.c_str()), 0, NULL) ;

	SessionServer sessionServer = SessionServer(sessionKey); ;
	UserServer userServer = UserServer(sessionServer);

	userServer.init();

	string userId = userServer.getData().getUserId();
	if( userId.size() == 0 ) {
		throw UMSVishnuException(ERRCODE_UNKNOWN_USER, "unable to assign a user to the session. May be the session is no longer valid.");
	}

	UMS_Data::ListMachineOptions mopts;
	mopts.setListAllMachine(false);
	mopts.setUserId(userId);
	mopts.setMachineId("");

	UMS_Data::ListMachines machines;
	vishnu::listMachines(sessionKey, machines, mopts) ;

	int machineCount = machines.getMachines().size() ;
	if( machineCount == 0) {
		throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE, "there is no machine assigned to this user: "+userId);
	}

	string machineId = "" ;
	long load = std::numeric_limits<long>::max();
	UMS_Data::Machine_ptr machine = machines.getMachines().get(0) ;
	for(int i=0; i< machineCount; i++) {
		UMS_Data::Machine_ptr machine = machines.getMachines().get(i) ;
		if(getMachineLoadPerformance(sessionKey, machine, criterion) < load) {
			machineId = machine->getMachineId();
		}
	}
	return machineId;
}


/**
 * \brief Function to compute the load performance of a given machine
 * \param sessionKey The session key
 * \param pb the request profile
 * \param the criteria of (number of waiting jobs, running jobs and total jobs)
 */
long
ServerTMS::getMachineLoadPerformance(const string& sessionKey, const UMS_Data::Machine_ptr& machine, const TMS_Data::LoadCriterion_ptr& criterion) {

	TMS_Data::ListJobs jobs ;
	TMS_Data::ListJobsOptions jobOtions ;
	vishnu::listJobs(sessionKey, machine->getMachineId(), jobs, jobOtions) ;
	long load = std::numeric_limits<long>::max();
	int criterionType = (criterion)? criterion->getLoadType(): jobs.getNbWaitingJobs() ;
	try {
		switch(criterionType) {
		case NBRUNNINGJOBS :
			load = jobs.getNbRunningJobs();
			break;
		case NBJOBS :
			load = jobs.getNbJobs();
			break;
		case NBWAITINGJOBS :
		default :
			load =jobs.getNbWaitingJobs();
			break;
		}
	} catch (VishnuException& ex) {
		std::cerr << ex.what() << std::endl;
	} catch(...) {
		std::cerr << "E: error while calculating the load performance of the machine "
				<< machine->getMachineId() << " (" << machine->getName() <<")"<< std::endl;
	}
	return load ;
}

