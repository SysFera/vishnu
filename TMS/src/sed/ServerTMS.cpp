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
		const std::string & machineId,
		BatchType batchType,
		const std::string & slaveBinDir
)
{

	//initialization of the batchType
	mbatchType = batchType;

	//initialization of the machineId
	mmachineId = machineId;

	//initialization of the slave directory
	mslaveBinDir = slaveBinDir;

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
			SystemException e(ERRCODE_DBERR, "vishnuid is unrecognized");
			throw e;
		}


	} catch (VishnuException& e) {
		std::cout << e.what() << std::endl;
		exit(0);
	}

	initMap(machineId);
	return 0;
}


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

