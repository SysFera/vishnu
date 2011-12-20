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
#include "utilServer.hpp"
#include "DbFactory.hpp"
#include "ScriptGenConvertor.hpp"

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

  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(mmachineId);
  MachineServer machineServer(machine);
  std::string machineName = machineServer.getMachineName();
  delete machine;

#if 0
  Env env(mbatchType);
  env.replaceEnvVariables(const_cast<std::string&>(scriptContent));
  env.replaceAllOccurences(const_cast<std::string&>(scriptContent), "$VISHNU_SUBMIT_MACHINE_NAME", machineName);
  env.replaceAllOccurences(const_cast<std::string&>(scriptContent), "${VISHNU_SUBMIT_MACHINE_NAME}", machineName);

  std::cout << "++++++++++++++++++++++BEGIN++++++++++++++++" << std::endl;
  std::cout << scriptContent << std::endl;
  std::cout << "++++++++++++++++++++++END++++++++++++++++" << std::endl;
#endif

  std::string jobSerialized ;
  std::string submitOptionsSerialized;
  char* scriptPath = NULL;
  ::ecorecpp::serializer::serializer optSer;
  ::ecorecpp::serializer::serializer jobSer;

  scriptPath = strdup("/tmp/job_scriptXXXXXX");

  std::string convertedScript;
  boost::shared_ptr<ScriptGenConvertor> scriptConvertor(vishnuScriptGenConvertor(mbatchType, scriptContent));
  if(scriptConvertor->scriptIsGeneric()) {
    std::string genScript = scriptConvertor->getConvertedScript();
    convertedScript = genScript;
  } else {
    convertedScript = scriptContent;
  }

  vishnu::createTmpFile(scriptPath, convertedScript);

  submitOptionsSerialized = optSer.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&options));
  jobSerialized =  jobSer.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

  SSHJobExec sshJobExec(acLogin, machineName, mbatchType, jobSerialized, submitOptionsSerialized);
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
  std::string vishnuJobId = vishnu::getObjectId(vishnuId, "jobcpt", "formatidjob", JOB, mmachineId);
  mjob.setJobId(vishnuJobId);

  string scriptContentStr = std::string(convertedScript);
  size_t pos = scriptContentStr.find("'");
  while(pos!=std::string::npos) {
    scriptContentStr.replace(pos, 1, " ");
    pos = scriptContentStr.find("'");
  }

  std::string numsession = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "numsessionid");
  std::string sqlInsert = "insert into job (vsession_numsessionid, submitMachineId, submitMachineName, jobId, batchJobId, batchType, jobName,"
    "jobPath, outputPath, errorPath, scriptContent, jobPrio, nbCpus, jobWorkingDir,"
    "status, submitDate, owner, jobQueue, wallClockLimit, groupName, jobDescription, memLimit,"
    "nbNodes, nbNodesAndCpuPerNode)"
    " values ("+numsession+",'"+mjob.getSubmitMachineId()+"','"+ mjob.getSubmitMachineName()+"','"+vishnuJobId+"','"
    +BatchJobId+"',"+convertToString(mbatchType)+",'"+mjob.getJobName()+"','"+mjob.getJobPath()+"','"
    +mjob.getOutputPath()+"','"+mjob.getErrorPath()+"','"
    +scriptContentStr+"',"+convertToString(mjob.getJobPrio())+","+convertToString(mjob.getNbCpus())+",'"
    +mjob.getJobWorkingDir()+"',"
    +convertToString(mjob.getStatus())+",CURRENT_TIMESTAMP,'"+mjob.getOwner()+"','"+mjob.getJobQueue()
    +"',"+convertToString(mjob.getWallClockLimit())+",'"+mjob.getGroupName()+"','"+mjob.getJobDescription()+"',"
    +convertToString(mjob.getMemLimit())
    +","+convertToString(mjob.getNbNodes())+",'"+mjob.getNbNodesAndCpuPerNode()+"')" ;

  mdatabaseVishnu->process(sqlInsert);

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
  time_t submitDate;
  time_t endDate;
  std::string sqlRequest = "SELECT vsessionid, submitMachineId, submitMachineName, jobId, jobName, jobPath,"
                                "outputPath, errorPath, jobPrio, nbCpus, jobWorkingDir, status, submitDate, endDate, owner,"
                                "jobQueue,wallClockLimit, groupName, jobDescription, memLimit, nbNodes, "
                                "nbNodesAndCpuPerNode from job, vsession "
                                "where vsession.numsessionid=job.vsession_numsessionid "
                                " and status > 0 and job.submitMachineId='"+mmachineId+"' and jobId='"+mjob.getJobId()+"'";

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
      mjob.setOutputPath(*(++iter));
      mjob.setErrorPath(*(++iter));
      mjob.setJobPrio(convertToInt(*(++iter)));
      mjob.setNbCpus(convertToInt(*(++iter)));
      mjob.setJobWorkingDir(*(++iter));
      mjob.setStatus(convertToInt(*(++iter)));
      //convert the submitDate into UTC date
      submitDate = convertLocaltimeINUTCtime(convertToTimeType(*(++iter)));
      mjob.setSubmitDate(submitDate);
      //convert the endDate into UTC date
      endDate = convertLocaltimeINUTCtime(convertToTimeType(*(++iter)));
      mjob.setEndDate(endDate);
      mjob.setOwner(*(++iter));
      mjob.setJobQueue(*(++iter));
      mjob.setWallClockLimit(convertToInt(*(++iter)));
      mjob.setGroupName(*(++iter));
      mjob.setJobDescription(*(++iter));
      mjob.setMemLimit(convertToInt(*(++iter)));
      mjob.setNbNodes(convertToInt(*(++iter)));
      mjob.setNbNodesAndCpuPerNode(*(++iter)); // a remplacer par
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
