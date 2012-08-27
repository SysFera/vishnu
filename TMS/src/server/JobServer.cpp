/**
 * \file JobServer.cpp
 * \brief This file contains the VISHNU JobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */
#include "JobServer.hpp"
#include "TMSVishnuException.hpp"
#include "LocalAccountServer.hpp"
#include "UserServer.hpp"
#include "SSHJobExec.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "DbFactory.hpp"
#include "ScriptGenConvertor.hpp"
#include <boost/algorithm/string.hpp>

#include "Env.hpp"

using namespace std;
/**
 * \param sessionServer The object which encapsulates the session information
 * \param machineId The machine identifier
 * \param job The job data structure
 * \param batchType The batch scheduler type
 * \brief Constructor
 */
JobServer::JobServer(const SessionServer& sessionServer,
		const std::string& machineId,
		const TMS_Data::Job& job,
		const BatchType& batchType):
		msessionServer(sessionServer), mmachineId(machineId), mjob(job), mbatchType(batchType) {

	DbFactory factory;
	mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \param vishnuId The VISHNU identifier
 * \param slaveDirectory the path to the TMS slave executable
 * \return raises an exception on error
 */
int JobServer::submitJob(const std::string& scriptContent,
		const TMS_Data::SubmitOptions& options,
		const int& vishnuId,
		const std::string& slaveDirectory)
{

	msessionServer.check(); //To check the sessionKey
	std::string acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);
	std::string vishnuJobId = vishnu::getObjectId(vishnuId, "formatidjob", JOB, mmachineId);
	std::string ouputDir = "" ;
	UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
	machine->setMachineId(mmachineId);
	MachineServer machineServer(machine);
	std::string machineName = machineServer.getMachineName();
	delete machine;

	Env env(mbatchType);
	std::string& scriptContentRef = const_cast<std::string&>(scriptContent) ;
	env.replaceEnvVariables(scriptContentRef);
	env.replaceAllOccurences(scriptContentRef, "$VISHNU_SUBMIT_MACHINE_NAME", machineName);
	env.replaceAllOccurences(scriptContentRef, "${VISHNU_SUBMIT_MACHINE_NAME}", machineName);

	if(options.getTextParams().size()) env.setParams(scriptContentRef, options.getTextParams()) ;
	if(options.getFileParams().size()) env.setParams(scriptContentRef, options.getFileParams()) ;

	mjob.setWorkId(options.getWorkId()) ;

	bool needOutputDir = false ;
	if(scriptContent.find("VISHNU_OUTPUT_DIR") != std::string::npos ) {
		std::string home = UserServer(msessionServer).getUserAccountProperty(mmachineId, "home");
		std::string workingDir = (!options.getWorkingDir().size())? home : options.getWorkingDir() ;
		std::string prefix = (boost::algorithm::ends_with(workingDir, "/"))? "OUTPUT_" : "/OUTPUT_" ;
		std::string dir = workingDir + prefix + vishnuJobId ;
		env.replaceAllOccurences(scriptContentRef, "$VISHNU_OUTPUT_DIR", dir);
		env.replaceAllOccurences(scriptContentRef, "${VISHNU_OUTPUT_DIR}", dir);
		mjob.setOutputDir(dir) ;
		needOutputDir = true ;
	}

	std::string jobSerialized ;
	std::string submitOptionsSerialized;
	char* scriptPath = NULL;
	::ecorecpp::serializer::serializer optSer;
	::ecorecpp::serializer::serializer jobSer;

	scriptPath = strdup("/tmp/job_scriptXXXXXX");

	std::string convertedScript;
	boost::shared_ptr<ScriptGenConvertor> scriptConvertor(vishnuScriptGenConvertor(mbatchType, scriptContentRef));
	if(scriptConvertor->scriptIsGeneric()) {
		std::string genScript = scriptConvertor->getConvertedScript();
		convertedScript = genScript;
	} else {
		convertedScript = scriptContentRef;
	}
	vishnu::createTmpFile(scriptPath, convertedScript);

	submitOptionsSerialized = optSer.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&options));
	jobSerialized =  jobSer.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

	SSHJobExec sshJobExec(acLogin, machineName, mbatchType, jobSerialized, submitOptionsSerialized);
	if( needOutputDir && sshJobExec.execCmd("mkdir " + mjob.getOutputDir())!=0) {
		throw SystemException(ERRCODE_SYSTEM, "Unable to set the job's output dir : " + mjob.getOutputDir()) ;
	}
	sshJobExec.sshexec(slaveDirectory, "SUBMIT", std::string(scriptPath));

	vishnu::deleteFile(scriptPath);

	std::string errorInfo = sshJobExec.getErrorInfo();
	if(errorInfo.size()!=0) {
		int code;
		std::string message;
		scanErrorMessage(errorInfo, code, message);
		throw TMSVishnuException(code, message);
	}

	std::string updateJobSerialized = sshJobExec.getJobSerialized();
	TMS_Data::Job_ptr job = NULL;
	if(!vishnu::parseEmfObject(std::string(updateJobSerialized), job)) {
		throw SystemException(ERRCODE_INVDATA, "JobServer::submitJob : job object is not well built");
	}
	mjob = *job;
	delete job;

	mjob.setSubmitMachineId(mmachineId);
	mjob.setSubmitMachineName(machineName);
	std::string sessionId = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "vsessionid");
	mjob.setSessionId(sessionId);

	std::string BatchJobId=mjob.getJobId();
	mjob.setJobId(vishnuJobId);

	string scriptContentStr = std::string(convertedScript);
	size_t pos = scriptContentStr.find("'");
	while(pos!=std::string::npos) {
		scriptContentStr.replace(pos, 1, " ");
		pos = scriptContentStr.find("'");
	}

	if(mbatchType==SGE){
		mjob.setOwner(acLogin);
	}

	std::string numsession = msessionServer.getAttribut("WHERE sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "numsessionid");
	//FIXME : Hack to take care about workid nullable ref. Externalize the ORM part
	std::string workId = "NULL" ;
	if(mjob.getWorkId() != 0){
		workId = convertToString(mjob.getWorkId()) ;
	}
	std::string sql = "INSERT INTO job (vsession_numsessionid, submitMachineId,"
			" submitMachineName, jobId, batchJobId, batchType, jobName,jobPath, outputPath, errorPath,"
			" scriptContent, jobPrio, nbCpus, jobWorkingDir, status, submitDate, owner, jobQueue, wallClockLimit,"
			" groupName, jobDescription, memLimit, nbNodes, nbNodesAndCpuPerNode, outputDir, workId)"
			" values (" + numsession + ",'" + mjob.getSubmitMachineId() + "','" + mjob.getSubmitMachineName() + "',"
			+ "'" + vishnuJobId + "','" + BatchJobId + "',"+ convertToString(mbatchType) + ",'" + mjob.getJobName()
			+ "','" + mjob.getJobPath() + "','" + mjob.getOutputPath() + "','" + mjob.getErrorPath()+"','" + "job" + "',"
			+ convertToString(mjob.getJobPrio()) + "," + convertToString(mjob.getNbCpus()) + ",'" + mjob.getJobWorkingDir() + "',"
			+ convertToString(mjob.getStatus()) + "," + "CURRENT_TIMESTAMP,'"+mjob.getOwner()+"','"+mjob.getJobQueue() + "',"
			+ convertToString(mjob.getWallClockLimit()) + ",'"+mjob.getGroupName() + "','" + mjob.getJobDescription()+"',"
			+ convertToString(mjob.getMemLimit()) + "," + convertToString(mjob.getNbNodes()) + ",'" + mjob.getNbNodesAndCpuPerNode() + "','"
			+ mjob.getOutputDir() + "'," + workId + ")";
	mdatabaseVishnu->process(sql);

	return 0;
}

/**
 * \brief Function to cancel job
 * \param slaveDirectory the path to the TMS slave executable
 * \return raises an exception on error
 */
int JobServer::cancelJob(const std::string& slaveDirectory)
{

	msessionServer.check(); //To check the sessionKey

	std::string acLogin;
	std::string machineName;
	std::string jobSerialized;
	std::string batchJobId;
	std::string initialJobId;
	std::string jobId;
	std::string owner;
	int status;
	std::vector<std::string> results;
	std::vector<std::string>::iterator  iter;
	acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);
	UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
	machine->setMachineId(mmachineId);
	MachineServer machineServer(machine);
	machineName = machineServer.getMachineName();
	delete machine;

	//Creation of the object user
	UserServer userServer = UserServer(msessionServer);
	userServer.init();

	std::string sqlCancelRequest;
	initialJobId = mjob.getJobId();
	if(initialJobId.compare("all")!=0 && initialJobId.compare("ALL")!=0) {
		sqlCancelRequest = "SELECT owner, status, jobId, batchJobId from job, vsession "
				"where vsession.numsessionid=job.vsession_numsessionid "
				" and jobId='"+mjob.getJobId()+"'";
	} else {
		if(!userServer.isAdmin()) {
			sqlCancelRequest = "SELECT owner, status, jobId, batchJobId from job, vsession "
					"where vsession.numsessionid=job.vsession_numsessionid and status < 5 and owner='"+acLogin+"'"
					" and submitMachineId='"+mmachineId+"'" ;
		} else {
			sqlCancelRequest = "SELECT owner, status, jobId, batchJobId from job, vsession "
					"where vsession.numsessionid=job.vsession_numsessionid and status < 5"
					" and submitMachineId='"+mmachineId+"'" ;
		}
	}


	boost::scoped_ptr<DatabaseResult> sqlCancelResult(mdatabaseVishnu->getResult(sqlCancelRequest.c_str()));
	if (sqlCancelResult->getNbTuples() != 0){
		for (size_t i = 0; i < sqlCancelResult->getNbTuples(); ++i) {
			results.clear();
			results = sqlCancelResult->get(i);
			iter = results.begin();

			owner = *iter;
			if(userServer.isAdmin()) {
				acLogin = owner;
			} else if(owner.compare(acLogin)!=0) {
				throw TMSVishnuException(ERRCODE_PERMISSION_DENIED);
			}

			++iter;
			status = convertToInt(*iter);
			if(status==5) {
				throw TMSVishnuException(ERRCODE_ALREADY_TERMINATED);
			}
			if(status==6) {
				throw TMSVishnuException(ERRCODE_ALREADY_CANCELED);
			}

			++iter;
			jobId = *iter;

			++iter;
			batchJobId = *iter;
			mjob.setJobId(batchJobId); //To reset the jobId

			::ecorecpp::serializer::serializer jobSer;
			jobSerialized =  jobSer.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

			SSHJobExec sshJobExec(acLogin, machineName, mbatchType, jobSerialized);
			sshJobExec.sshexec(slaveDirectory, "CANCEL");

			std::string errorInfo = sshJobExec.getErrorInfo();

			if(errorInfo.size()!=0 && (initialJobId.compare("all")!=0 && initialJobId.compare("ALL")!=0)) {
				int code;
				std::string message;
				scanErrorMessage(errorInfo, code, message);
				throw TMSVishnuException(code, message);
			} else if(errorInfo.size()==0) {

				std::string sqlUpdatedRequest = "UPDATE job SET status=6 where jobId='"+jobId+"'";
				mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
			}
		}
	} else {
		if(initialJobId.compare("all")!=0 && initialJobId.compare("ALL")!=0) {
			throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
		}
	}

	return 0;
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */
TMS_Data::Job JobServer::getJobInfo() {

	//To check the sessionKey
	msessionServer.check();

	std::vector<std::string> results;
	std::vector<std::string>::iterator  iter;
	std::string sqlRequest =
			"SELECT vsessionid, submitMachineId, submitMachineName, jobId, jobName, jobPath, workId, "
			"  outputPath, errorPath, outputDir, jobPrio, nbCpus, jobWorkingDir, job.status, "
			"  submitDate, endDate, owner, jobQueue,wallClockLimit, groupName, jobDescription, "
			"  memLimit, nbNodes, nbNodesAndCpuPerNode, batchJobId, userid"
			" FROM job, vsession, users "
			" WHERE vsession.numsessionid=job.vsession_numsessionid "
			" AND vsession.users_numuserid=users.numuserid"
			" AND job.status > 0 and job.submitMachineId='"+mmachineId+"'"
			" AND job.jobId='"+mjob.getJobId()+"'";

	boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseVishnu->getResult(sqlRequest.c_str()));

	if (sqlResult->getNbTuples() != 0){
		results.clear();
		results = sqlResult->get(0);
		iter = results.begin();

		mjob.setSessionId(*iter);
		mjob.setSubmitMachineId(*(++iter));
		mjob.setSubmitMachineName(*(++iter));
		mjob.setJobId(*(++iter));
		mjob.setJobName(*(++iter));
		mjob.setJobPath(*(++iter));
		mjob.setWorkId(convertToLong(*(++iter)));
		mjob.setOutputPath(*(++iter));
		mjob.setErrorPath(*(++iter));
		mjob.setOutputDir(*(++iter));
		mjob.setJobPrio(convertToInt(*(++iter)));
		mjob.setNbCpus(convertToInt(*(++iter)));
		mjob.setJobWorkingDir(*(++iter));
		mjob.setStatus(convertToInt(*(++iter)));
		mjob.setSubmitDate( convertLocaltimeINUTCtime(convertToTimeType(*(++iter))) ); //convert the submitDate into UTC date
		mjob.setEndDate(convertLocaltimeINUTCtime(convertToTimeType(*(++iter)))); //convert the endDate into UTC date
		mjob.setOwner(*(++iter));
		mjob.setJobQueue(*(++iter));
		mjob.setWallClockLimit(convertToInt(*(++iter)));
		mjob.setGroupName(*(++iter));
		mjob.setJobDescription(*(++iter));
		mjob.setMemLimit(convertToInt(*(++iter)));
		mjob.setNbNodes(convertToInt(*(++iter)));
		mjob.setNbNodesAndCpuPerNode(*(++iter));
		mjob.setBatchJobId(*(++iter));
		mjob.setUserId(*(++iter));
	} else {
		throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
	}

	return mjob;
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */
TMS_Data::Job JobServer::getData()
{
	return mjob;
}

/**
 * \brief Function to scan VISHNU error message
 * \param errorInfo the error information to scan
 * \param code The code The code of the error
 * \param message The message associeted to the error code
 * \return raises an exception on erroor
 */
void JobServer::scanErrorMessage(const std::string& errorInfo, int& code, std::string& message) {

	code = ERRCODE_INVEXCEP;

	size_t pos = errorInfo.find('#');
	if(pos!=std::string::npos) {
		std::string codeInString = errorInfo.substr(0,pos);
		if(codeInString.size()!=0) {
			std::istringstream isCode(codeInString);
			isCode >> code;
			message = errorInfo.substr(pos+1);
		}
	}
}

/**
 * \brief Function to convert a given date into correspondant long value
 * \param date The date to convert
 * \return The converted value
 */
long long JobServer::convertToTimeType(std::string date) {

	if(date.size()==0 ||
			// For mysql, the empty date is 0000-00-00, not empty, need this test to avoid problem in ptime
			date.find("0000-00-00")!=std::string::npos) {
		return 0;
	}

	boost::posix_time::ptime pt(time_from_string(date));
	boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
	time_duration::sec_type time = (pt - epoch).total_seconds();

	return (long long) time_t(time);

}


/**
 * \brief Destructor
 */
JobServer::~JobServer() {

}
