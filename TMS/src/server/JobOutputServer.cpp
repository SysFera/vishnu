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
	std::string sqlRequest = "SELECT outputPath, errorPath, owner, status, submitDate, outputDir from vsession, job where"
			" vsession.numsessionid=job.vsession_numsessionid and jobId='"+mjobResult.getJobId()+"'"
			" and submitMachineId='"+mmachineId+"'" ;

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

	if(owner.compare(acLogin)!=0) {
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

	char* copyOfOutputPath = strdup("/tmp/job_outputPathXXXXXX");
	char* copyOfErrorPath = strdup("/tmp/job_errorPathXXXXXX");
	vishnu::createTmpFile(copyOfOutputPath);
	vishnu::createTmpFile(copyOfErrorPath);

	SSHJobExec sshJobExec(acLogin, machineName);
	if(sshJobExec.copyFiles(outputPath, errorPath , copyOfOutputPath, copyOfErrorPath)){
		vishnu::deleteFile(copyOfOutputPath);
		vishnu::deleteFile(copyOfErrorPath);
		time_t submitDate = convertLocaltimeINUTCtime(convertToTimeType(subDateStr));
		if(vishnu::getCurrentTimeInUTC()-submitDate > 648000) {
			std::string sqlUpdatedRequest = "UPDATE job SET status=7 where jobId='"+mjobResult.getJobId()+"'";
			mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
		}
		throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::copyFiles: problem to get the output or error file on this user local account");
	}

	mjobResult.setOutputPath(std::string(copyOfOutputPath));
	mjobResult.setErrorPath(std::string(copyOfErrorPath));
	mjobResult.setOutputDir(outputDir) ;

	time_t submitDate = convertLocaltimeINUTCtime(convertToTimeType(subDateStr));
	if(vishnu::getCurrentTimeInUTC()-submitDate > 2592000) {
		std::string sqlUpdatedRequest = "UPDATE job SET status=7 where jobId='"+mjobResult.getJobId()+"'";
		mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
	}

	return mjobResult;
}

/**
 * \brief Function to get the all completed jobs results
 * \param jobId The Id of the
 * \return The lits of job results data structure
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
	std::string jobId;
	int status;
	std::vector<std::string> results;
	std::vector<std::string>::iterator  iter;

	TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
	mlistJobsResult = ecoreFactory->createListJobResults();

	//To get the output and error path of all jobs
	std::string sqlRequest = "SELECT jobId, outputPath, errorPath, status, submitDate from vsession, job where"
			" vsession.numsessionid=job.vsession_numsessionid and owner='"+acLogin+"'"
			" and submitMachineId='"+mmachineId+"'" ;
	boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseVishnu->getResult(sqlRequest.c_str()));

	if (sqlResult->getNbTuples() != 0){
		for (size_t i = 0; i < sqlResult->getNbTuples(); ++i) {
			TMS_Data::JobResult_ptr out = ecoreFactory->createJobResult();
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

			size_t pos1 = outputPath.find(":");
			if(pos1!=std::string::npos) {
				outputPath = outputPath.substr(pos1+1);
			}
			size_t pos2 = errorPath.find(":");
			if(pos2!=std::string::npos) {
				errorPath = errorPath.substr(pos2+1);
			}

			if((outputPath.size()!=0) && errorPath.size()!=0 && status==5) {
				char* copyOfOutputPath = strdup("/tmp/job_outputPathXXXXXX");
				char* copyOfErrorPath = strdup("/tmp/job_errorPathXXXXXX");

				vishnu::createTmpFile(copyOfOutputPath);
				vishnu::createTmpFile(copyOfErrorPath);

				SSHJobExec sshJobExec(acLogin, machineName);
				if(!sshJobExec.copyFiles(outputPath, errorPath , copyOfOutputPath, copyOfErrorPath)) {;

				out->setJobId(jobId);
				out->setOutputPath(std::string(copyOfOutputPath));
				out->setErrorPath(std::string(copyOfErrorPath));

				mlistJobsResult->getResults().push_back(out);

				time_t submitDate = convertLocaltimeINUTCtime(convertToTimeType(*(++iter)));
				if(vishnu::getCurrentTimeInUTC()-submitDate > 2592000) {
					std::string sqlUpdatedRequest = "UPDATE job SET status=7 where jobId='"+jobId+"'";
					mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
				}
				} else {
					vishnu::deleteFile(copyOfOutputPath);
					vishnu::deleteFile(copyOfErrorPath);
					time_t submitDate = convertLocaltimeINUTCtime(convertToTimeType(*(++iter)));
					if(vishnu::getCurrentTimeInUTC()-submitDate > 648000) {
						std::string sqlUpdatedRequest = "UPDATE job SET status=7 where jobId='"+jobId+"'";
						mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
					}
				}
			}
		}
		mlistJobsResult->setNbJobs(mlistJobsResult->getResults().size());
	}

	return mlistJobsResult;
}

/**
 * \brief Destructor
 */
JobOutputServer::~JobOutputServer() {

}
