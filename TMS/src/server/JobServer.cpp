/**
  * \file JobServer.cpp
  * \brief This file contains the VISHNU JobServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */
#include "JobServer.hpp"
#include "emfTMSUtils.hpp"
#include "TMSVishnuException.hpp"

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
  char* scriptPath = NULL;
  std::string jobSerialized ;
  std::string submitOptionsSerialized; 
  const char* name = "submit";
  ::ecorecpp::serializer::serializer optSer(name);
  ::ecorecpp::serializer::serializer jobSer(name);

  scriptPath = const_cast<char*>((std::string(getenv("HOME"))+"/job_script").c_str());
  SSHJobExec().createTmpFile(scriptPath, scriptContent);

  std::ofstream ofile(scriptPath);
  ofile << scriptContent;
  ofile.close();

  submitOptionsSerialized = optSer.serialize(const_cast<TMS_Data::SubmitOptions_ptr>(&options));
  jobSerialized =  jobSer.serialize(const_cast<TMS_Data::Job_ptr>(&mjob));
 
  SSHJobExec sshJobExec(scriptPath, jobSerialized, submitOptionsSerialized, mbatchType);
  sshJobExec.sshexec("SUBMIT");

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
    SSHJobExec().deleteFile(scriptPath);
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "JobServer::submitJob : job object is not well built");
  }
  mjob = *job;
  SSHJobExec().deleteFile(scriptPath);
  delete job;

  return 0;
}

/**
 * \brief Function to cancel job
 * \return raises an exception on error
 */ 
int JobServer::cancelJob()
{
  std::string jobSerialized;
  const char* name = "submit";
  ::ecorecpp::serializer::serializer jobSer(name);

  jobSerialized =  jobSer.serialize(const_cast<TMS_Data::Job_ptr>(&mjob));

  SSHJobExec sshJobExec(NULL, jobSerialized, "", mbatchType);
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


/**
 * \brief Destructor
 */
JobServer::~JobServer() {

}
