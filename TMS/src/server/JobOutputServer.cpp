/**
 * \file JobServer.cpp
 * \brief This file contains the VISHNU JobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */
#include "JobOutputServer.hpp"
#include "TMSVishnuException.hpp"
#include "LocalAccountServer.hpp"
#include "UserServer.hpp"
#include "SSHJobExec.hpp"
#include "utilServer.hpp"
#include "DbFactory.hpp"

/**
 * \param session The object which encapsulates the session information
 * \param machineid The machine identifier 
 * \brief Constructor
 */
JobOutputServer::JobOutputServer(const SessionServer& sessionServer,
		const std::string& machineId):
		msessionServer(sessionServer), mmachineId(machineId)
{
	mlistJobsResult = NULL;
	DbFactory factory;
	mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
 * \param session The object which encapsulates the session information
 * \param machineid The machine identifier
 * \param jobResult The job result data structure
 * \brief Constructor
 */
JobOutputServer::JobOutputServer(const SessionServer& sessionServer,
		const std::string& machineId,
		const TMS_Data::JobResult& jobResult):
		msessionServer(sessionServer), mmachineId(machineId), mjobResult(jobResult)
{
	mlistJobsResult = NULL;
	DbFactory factory;
	mdatabaseVishnu = factory.getDatabaseInstance();
}
/**
 * \brief Function to get the job results
 * \param jobId The Id of the
 * \return The job results data structure
 */
TMS_Data::JobResult
JobOutputServer::getJobOutput() {

	//To check the sessionKey
	msessionServer.check();

	std::string acLogin;
	std::string machineName;
	std::string outputPath;
	std::string errorPath;
	std::string owner;
	std::string subDateStr;
	std::string outputDir;
	int status;
	std::vector<std::string> results;
	std::vector<std::string>::iterator  iter;
	//To get the output and error path of the job
	std::string sqlRequest = "SELECT outputPath, errorPath, owner, status, submitDate, outputDir "
			"FROM vsession, job "
			"WHERE vsession.numsessionid=job.vsession_numsessionid"
			"  AND job.jobId='"+mjobResult.getJobId()+"' "
			"  AND job.submitMachineId='"+mmachineId+"'" ;

	boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseVishnu->getResult(sqlRequest.c_str()));

	acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);

	UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
	machine->setMachineId(mmachineId);
	MachineServer machineServer(machine);
	machineName = machineServer.getMachineName();
	delete machine;

	if(sqlResult->getNbTuples() == 0) {
		throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
	}

	results.clear();
	results = sqlResult->get(0);
	iter = results.begin();
	outputPath = *iter;
	++iter;
	errorPath = *iter;
	++iter;
	owner = *iter;
	++iter;
	status = convertToInt(*iter);
	++iter;
	subDateStr = *iter;
	++iter;
	outputDir = *iter;

	if( owner.compare(acLogin) != 0 ) {
		throw TMSVishnuException(ERRCODE_PERMISSION_DENIED, "You can't get the output of "
				"this job because it is for an other owner");
	}

	switch(status) {
	case 7: throw TMSVishnuException(ERRCODE_ALREADY_DOWNLOADED); break ;
	case 6: throw TMSVishnuException(ERRCODE_ALREADY_CANCELED); break ;
	case 5: break ; //Job is terminated and get have output
	default: throw TMSVishnuException(ERRCODE_JOB_IS_NOT_TERMINATED); //status would be less than 5
	}

	outputPath = outputPath.substr(outputPath.find(":")+1);
	errorPath = errorPath.substr(errorPath.find(":")+1);

	if(outputPath.size()==0 || errorPath.size() ==0 ) {
		throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
	}

	mjobResult.setOutputDir( outputDir ) ;
	mjobResult.setOutputPath( outputPath) ;
	mjobResult.setErrorPath( errorPath) ;

	return mjobResult;
}

/**
 * \brief Function to get the all completed jobs results
 * \return The list of job results data structure
 */
TMS_Data::ListJobResults_ptr
JobOutputServer::getCompletedJobsOutput() {

	//To check the sessionKey
	msessionServer.check();

	std::string acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);

	UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
	machine->setMachineId(mmachineId);
	MachineServer machineServer(machine);
	std::string machineName = machineServer.getMachineName();
	delete machine;

	std::string outputPath;
	std::string errorPath;
	std::string subDateStr;
	std::string outputDir;
	std::string jobId;
	int status;
	std::vector<std::string> results;
	std::vector<std::string>::iterator  iter;

	TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
	mlistJobsResult = ecoreFactory->createListJobResults();

	//To get the output and error path of all jobs
	std::string sqlRequest = "SELECT jobId, outputPath, errorPath, status, submitDate, outputDir "
			"FROM vsession, job "
			"WHERE vsession.numsessionid=job.vsession_numsessionid"
			"  AND job.owner='"+acLogin+"'"
			"  AND job.submitMachineId='"+mmachineId+"'"
			"  AND job.status=5" ;
	boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseVishnu->getResult(sqlRequest.c_str()));

	if (sqlResult->getNbTuples() == 0) {
		return mlistJobsResult;
	}

	for (size_t i = 0; i < sqlResult->getNbTuples(); ++i) {
		results.clear();
		results = sqlResult->get(i);
		iter = results.begin();

		jobId = *iter;
		++iter;
		outputPath = *iter;
		++iter;
		errorPath = *iter;
		++iter;
		status = convertToInt(*iter);
		++iter;
		subDateStr = *iter;
		++iter;
		outputDir = *iter;

		size_t pos1 = outputPath.find(":");
		if( pos1 != std::string::npos ) {
			outputPath = outputPath.substr(pos1+1);
		}
		size_t pos2 = errorPath.find(":");
		if( pos2 != std::string::npos ) {
			errorPath = errorPath.substr(pos2+1);
		}

		TMS_Data::JobResult_ptr curResult = ecoreFactory->createJobResult();
		curResult->setJobId(jobId);
		curResult->setOutputDir( outputDir ) ;
		curResult->setOutputPath(outputPath) ;
		curResult->setErrorPath( errorPath) ;
		mlistJobsResult->getResults().push_back(curResult);

		time_t submitDate = convertLocaltimeINUTCtime(convertToTimeType(subDateStr));
		if( vishnu::getCurrentTimeInUTC()-submitDate > 2592000 ) {  // Retention of 1 month
			std::string sqlUpdatedRequest = "UPDATE job SET status=7 where jobId='"+jobId+"'";
			mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
		}

	}
	mlistJobsResult->setNbJobs(mlistJobsResult->getResults().size());

	return mlistJobsResult;
}

/**
 * \brief Destructor
 */
JobOutputServer::~JobOutputServer() {

}
