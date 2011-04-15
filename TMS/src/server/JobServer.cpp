/**
  * \file JobServer.cpp
  * \brief This file contains the VISHNU JobServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */
#include "JobServer.hpp"
#include "emfTMSUtils.hpp"
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
  if(!vishnu::parseTMSEmfObject(std::string(updateJobSerialized), job)) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "JobServer::submitJob : job object is not well built");
  }
  mjob = *job;
  std::string BatchJobId=mjob.getJobId();
  std::cout << "BatchJobId=" << BatchJobId << std::endl;
  std::cout << "ServerTMS::getInstance()->getVishnuId()=" << ServerTMS::getInstance()->getVishnuId() << std::endl;
  std::string vishnuJobId = vishnu::getObjectId(ServerTMS::getInstance()->getVishnuId(), "jobcpt", "formatidjob", JOB, mmachineId);
  std::cout << "vishnuJobId = " << vishnuJobId << std::endl;
  //mjob.setJobId(vishnuJobId);
  delete job;

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
