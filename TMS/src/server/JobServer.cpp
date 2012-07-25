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
                const std::string& slaveDirectory,
                const std::vector<std::string>& defaultBatchOption)
{

	msessionServer.check(); //To check the sessionKey
	std::string acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);
	std::string vishnuJobId = vishnu::getObjectId(vishnuId, "formatidjob", JOB, mmachineId);
	std::string ouputDir = "" ;
        int count=0;
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
		std::string dir = workingDir + prefix + vishnuJobId + vishnu::createSuffixFromCurTime() ;
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
	std::string key;
        std::string sep = " ";
        switch(mbatchType) {
          case TORQUE :
            key = "#PBS";
            break;
          case LOADLEVELER :
            key = "# @";
            sep = " = ";
            break;
          case SLURM :
            key = "#SBATCH";
            break;
          case LSF :
            key = "#BSUB";
            break;
          case SGE :
            key = "#$";
            break;
          case PBSPRO :
            key = "#PBS";
            break;
          default :
            break;
        }
        if(options.getSpecificParams().size()){

          std::string specificParams = options.getSpecificParams();
          
          size_t pos1 =0;
          size_t pos2=0;
          pos2 = specificParams.find("=");
          while (pos2!=std::string::npos){
            size_t pos3=0;
            pos3 = specificParams.find(" ");
            if(pos3!=std::string::npos){
                          
              std::string lineoption = key +" "+ specificParams.substr(pos1, pos2-pos1)+ sep +  specificParams.substr(pos2+1, pos3-pos2) + "\n";
              insertOptionLine(lineoption, convertedScript, key);
              specificParams.erase(0,pos3);
              boost::algorithm::trim_left(specificParams);
                            
            } else {
              std::string lineoption = key +" "+ specificParams.substr(pos1, pos2-pos1)+ sep +  specificParams.substr(pos2+1, specificParams.size()-pos2) + "\n";
              insertOptionLine(lineoption, convertedScript, key);
              break;
            }
            pos2 = specificParams.find("=");
            
            
          }
        
        }
        if (defaultBatchOption.size()){
          processDefaultOptions(defaultBatchOption, convertedScript, key);
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
	std::string workId = (mjob.getWorkId() != 0)? convertToString(mjob.getWorkId()) : "NULL" ;

	pos = mjob.getOutputPath().find(":");
	std::string prefixOutputPath = (pos == std::string::npos)? mjob.getSubmitMachineName()+":" : "";
	pos = mjob.getErrorPath().find(":");
	std::string prefixErrorPath = (pos == std::string::npos)? mjob.getSubmitMachineName()+":" : "";

	std::string sqlUpdate = "UPDATE job set ";
	sqlUpdate+="vsession_numsessionid="+numsession+",";
	sqlUpdate+="submitMachineId='"+mjob.getSubmitMachineId()+"',";
	sqlUpdate+="submitMachineName='"+mjob.getSubmitMachineName()+"',";
	sqlUpdate+="batchJobId='"+BatchJobId+"',";
	sqlUpdate+="batchType="+convertToString(mbatchType)+",";
	sqlUpdate+="jobName='"+mjob.getJobName()+"',";
	sqlUpdate+="jobPath='"+mjob.getJobPath()+"',";
	sqlUpdate+="outputPath='"+prefixOutputPath+mjob.getOutputPath()+"',";
	sqlUpdate+="errorPath='"+prefixErrorPath+mjob.getErrorPath()+"',";
	sqlUpdate+="scriptContent='job',";
	sqlUpdate+="jobPrio="+convertToString(mjob.getJobPrio())+",";
	sqlUpdate+="nbCpus="+convertToString(mjob.getNbCpus())+",";
	sqlUpdate+="jobWorkingDir='"+mjob.getJobWorkingDir()+"',";
	sqlUpdate+="status="+convertToString(mjob.getStatus())+",";
	sqlUpdate+="submitDate=CURRENT_TIMESTAMP,";
	sqlUpdate+="owner='"+mjob.getOwner()+"',";
	sqlUpdate+="jobQueue='"+mjob.getJobQueue()+"',";
	sqlUpdate+="wallClockLimit="+convertToString(mjob.getWallClockLimit())+",";
	sqlUpdate+="groupName='"+mjob.getGroupName()+"',";
	sqlUpdate+="jobDescription='"+mjob.getJobDescription()+"',";
	sqlUpdate+="memLimit="+convertToString(mjob.getMemLimit())+",";
	sqlUpdate+="nbNodes="+convertToString(mjob.getNbNodes())+",";
	sqlUpdate+="nbNodesAndCpuPerNode='"+mjob.getNbNodesAndCpuPerNode()+"',";
	sqlUpdate+="outputDir='"+mjob.getOutputDir()+"' ";
	sqlUpdate+=",workId="+workId+" ";
	sqlUpdate+="WHERE jobid='"+vishnuJobId+"';";

	mdatabaseVishnu->process(sqlUpdate);


	return 0;
}

/**
 * \brief Function to treat the default submission options
 * \param scriptOptions The list of the option value
 * \param cmdsOptions The list of the option value
 * \return raises an exception on error
 */
void
JobServer::processDefaultOptions(const std::vector<std::string>& defaultBatchOption, std::string& content, std::string& key){
  
  size_t pos = 0;
  size_t position =0;
  size_t posLastDirective = 0;
  std::string key1, key2;
    int count = 0;
  while(count < defaultBatchOption.size()) {

    key1 =  defaultBatchOption.at(count);
    position = 0;
    int found =0;
    while(position!=string::npos && !found){
      position = content.find(key.c_str(), position);
      if(position!=std::string::npos){
        size_t pos1 = content.find("\n", position);
        std::string line = content.substr(position, pos1-position);
        position++;
        size_t pos2 = line.find(key1.c_str());
        if(pos2 != std::string::npos){
           found =1;
           break;
        }
      }
    }  
    if(!found){
      std::string lineoption = key + " " + defaultBatchOption.at(count) + " " + defaultBatchOption.at(count +1) + "\n";
      insertOptionLine(lineoption, content, key);
    }
    count +=2;
  }
  
}
/**
 * \brief Function to insert option into string
 * \param optionLineToInsert the option to insert
 * \param content The buffer containing the inserted option
 * \return raises an exception on error
 */
void
JobServer::insertOptionLine( std::string& optionLineToInsert,
                             std::string& content, std::string& key) {
  
  size_t pos = 0;
  int found=0;
  size_t posLastDirective = 0;
  
  while(pos!=string::npos) {
    
    pos = content.find(key.c_str(), pos);
    if(pos!=string::npos) {
      size_t pos1 = 0;
      pos1 = content.find("\n", pos);
      while (content.compare(pos1-1,1,"\\") == 0){
        pos1 = content.find("\n", pos1 + 1);
      }
      std::string line = content.substr(pos, pos1-pos);
      if(content.compare(pos-1,1,"\n")==0){
        if(mbatchType==LOADLEVELER){
          std::string line_tolower(line);
          std::transform(line.begin(), line.end(), line_tolower.begin(), ::tolower);
          if(line_tolower.find("queue")!=string::npos) {
            break;
          }
        }
        posLastDirective = pos + line.size() +1;         
      }
      pos++;    
    }
  }
  content.insert(posLastDirective, optionLineToInsert);
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
