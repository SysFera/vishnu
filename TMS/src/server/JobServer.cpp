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
#include "ServerTMS.hpp"

/**
 * \param session The object which encapsulates the session information
 * \param machineId The machine identifier
 * \param job The job data structure
 * \brief Constructor
 */
JobServer::JobServer(const SessionServer& sessionServer,
                     const std::string& machineId,
                     const TMS_Data::Job& job,
                     const BatchType& batchType):
  msessionServer(sessionServer), mmachineId(machineId), mjob(job), mbatchType(batchType) {

}

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \return raises an exception on error
 */
int JobServer::submitJob(const std::string& scriptContent, const TMS_Data::SubmitOptions& options)
{

  msessionServer.check(); //To check the sessionKey

  std::string acLogin = getUserAccountLogin();
  std::cout << "acLogin = " << acLogin << std::endl;

  std::string machineName = getMachineName();
  std::cout << "machineName = " << machineName << std::endl;

  std::string jobSerialized ;
  std::string submitOptionsSerialized;
  char* scriptPath = NULL;
  const char* name = "submit";
  ::ecorecpp::serializer::serializer optSer(name);
  ::ecorecpp::serializer::serializer jobSer(name);

  scriptPath = strdup("/tmp/job_scriptXXXXXX");
  SSHJobExec().createTmpFile(scriptPath, scriptContent);

  std::ofstream ofile(scriptPath);
  ofile << scriptContent;
  ofile.close();

  submitOptionsSerialized = optSer.serialize(const_cast<TMS_Data::SubmitOptions_ptr>(&options));
  jobSerialized =  jobSer.serialize(const_cast<TMS_Data::Job_ptr>(&mjob));

  SSHJobExec sshJobExec(scriptPath, jobSerialized, submitOptionsSerialized, acLogin, machineName, "", mbatchType);
  sshJobExec.sshexec("SUBMIT");

  SSHJobExec().deleteFile(scriptPath);

  std::string errorInfo = sshJobExec.getErrorInfo();

  if(errorInfo.size()!=0) {
     int code;
     std::string message;
     scanErrorMessage(errorInfo, code, message);
     throw TMSVishnuException(code, message);
  }

  std::string updateJobSerialized = sshJobExec.getJobSerialized().c_str();
  TMS_Data::Job_ptr job = NULL;
  if(!vishnu::parseEmfObject(std::string(updateJobSerialized), job)) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "JobServer::submitJob : job object is not well built");
  }
  mjob = *job;
  delete job;

  mjob.setSubmitMachineId(mmachineId);
  mjob.setSubmitMachineName(machineName);
  std::string sessionId = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "vsessionid");
  mjob.setSessionId(sessionId);

  std::string BatchJobId=mjob.getJobId();
  std::string vishnuJobId = vishnu::getObjectId(ServerTMS::getInstance()->getInstance()->getVishnuId(), "jobcpt", "formatidjob", JOB, mmachineId);
  mjob.setJobId(vishnuJobId);

  Database* databaseVishnu = ServerTMS::getInstance()->getDatabaseVishnu();
  std::string numsession = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "numsessionid");
  std::string sqlInsert = "insert into job (vsession_numsessionid, submitMachineId, submitMachineName, jobId, batchJobId, batchType, jobName,"
    "jobPath, outputPath, errorPath, scriptContent, jobPrio, nbCpus, jobWorkingDir,"
    "status, submitDate, endDate, owner, jobQueue, wallClockLimit, groupName, jobDescription, memLimit,"
    "nbNodes, nbNodesAndCpuPerNode)"
    " values ("+numsession+",'"+mjob.getSubmitMachineId()+"','"+ mjob.getSubmitMachineName()+"','"+vishnuJobId+"','"
    +BatchJobId+"',"+convertToString(mbatchType)+",'"+mjob.getJobName()+"','"+mjob.getJobPath()+"','"
    +mjob.getOutputPath()+"','"+mjob.getErrorPath()+"','"
    +std::string(scriptContent)+"',"+convertToString(mjob.getJobPrio())+","+convertToString(mjob.getNbCpus())+",'"
    +mjob.getJobWorkingDir()+"',"
    +convertToString(mjob.getStatus())+",CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,'"+mjob.getOwner()+"','"+mjob.getJobQueue()
    +"',"+convertToString(mjob.getWallClockLimit())+",'"+mjob.getGroupName()+"','"+mjob.getJobDescription()+"',"
    +convertToString(mjob.getMemLimit())
    +","+convertToString(mjob.getNbNodes())+","+"'1:1'"+")" ;  //TODO : a ajouter mjob.getNbNodesAndCpuPerNode() 

  std::cout << sqlInsert << std::endl;
  databaseVishnu->process(sqlInsert); 
    
  return 0;
}

/**
 * \brief Function to cancel job
 * \return raises an exception on error
 */
int JobServer::cancelJob()
{

  msessionServer.check(); //To check the sessionKey

  std::string jobSerialized;
  const char* name = "submit";
  ::ecorecpp::serializer::serializer jobSer(name);

  jobSerialized =  jobSer.serialize(const_cast<TMS_Data::Job_ptr>(&mjob));

  std::string acLogin = getUserAccountLogin();
  std::cout << "acLogin = " << acLogin << std::endl;

  std::string machineName = getMachineName();
  std::cout << "machineName = " << machineName << std::endl;

  SSHJobExec sshJobExec(NULL, jobSerialized, "", acLogin, machineName, "", mbatchType);
  sshJobExec.sshexec("CANCEL");

  std::string errorInfo = sshJobExec.getErrorInfo();

  if(errorInfo.size()!=0) {
    int code;
    std::string message;
    scanErrorMessage(errorInfo, code, message);
    throw TMSVishnuException(code, message);
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

  std::string outputPath;
  std::string errorPath;
  std::vector<std::string> results;
  std::vector<std::string>::iterator  iter;
  std::string sqlRequest = "SELECT vsessionid, submitMachineId, submitMachineName, jobId, jobName, jobPath, errorPath,"
                                "outputPath, jobPrio, nbCpus, jobWorkingDir, status, submitDate, endDate, owner,"
                                "jobQueue,wallClockLimit, groupName, jobDescription, memLimit, nbNodes, "
                                "nbNodesAndCpuPerNode from job, vsession "
                                "where vsession.numsessionid=job.vsession_numsessionid"
                                " and job.submitMachineId='"+mmachineId+"' and jobId='"+mjob.getJobId()+"'";

  boost::scoped_ptr<DatabaseResult> sqlResult(ServerTMS::getInstance()->getDatabaseVishnu()->getResult(sqlRequest.c_str()));
  
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
      mjob.setSubmitDate(string_to_time_t(*(++iter)));
      mjob.setEndDate(string_to_time_t(*(++iter)));
      mjob.setOwner(*(++iter));
      mjob.setJobQueue(*(++iter));
      mjob.setWallClockLimit(convertToInt(*(++iter)));
      mjob.setGroupName(*(++iter));
      mjob.setJobDescription(*(++iter));
      mjob.setMemLimit(convertToInt(*(++iter)));
      mjob.setNbNodes(convertToInt(*(++iter)));
      mjob.setNbNodesAndCpuPerNode(1); // a remplacer par  
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
 * \brief Function to scan the error message
 * \param errorInfo The error message to scan
 * \param code The code of errorInfo
 * \param message The message of errorInfo
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

std::string JobServer::getUserAccountLogin() {

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  std::string userId = (userServer.getData()).getUserId();
  UMS_Data::LocalAccount_ptr account = new UMS_Data::LocalAccount();
  account->setMachineId(mmachineId);
  account->setUserId(userId);
  LocalAccountServer localAccount(account, msessionServer);

  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(mmachineId);
  MachineServer machineServer(machine);

  //To get the database number id of the machine
  std::string numMachine = machineServer.getAttribut("where machineid='"+localAccount.getData()->getMachineId()+"'");
  //To get the database number id of the user
  std::string numUser = userServer.getAttribut("where userid='"+localAccount.getData()->getUserId()+"'");

  std::string acLogin;
  if ((numMachine.size() > 0) && (numUser.size() > 0)) {
    acLogin = localAccount.getAttribut("where machine_nummachineid="+numMachine+" and users_numuserid="+numUser, "aclogin");
  }

  if(acLogin.size()==0) {
    delete account;
    delete machine;
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "You have not a local account on this machine");
  }

  delete account;
  delete machine;
  return acLogin;
}

std::string JobServer::getMachineName() {

  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(mmachineId);
  MachineServer machineServer(machine);

  std::string  machineName = machineServer.getAttribut("where machineid='"+machineServer.getData()->getMachineId()+"'", "name");

  delete machine;

  return machineName;
}

/**
 * \brief Destructor
 */
JobServer::~JobServer() {

}
