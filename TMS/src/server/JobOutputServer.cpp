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
#include "ServerTMS.hpp"

//Just a test
#include "JobServer.hpp"

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

  std::string outputPath;
  std::string errorPath;
  std::string owner;
  int status;
  std::vector<std::string> results;
  std::vector<std::string>::iterator  iter;
  //To get the output and error path of the job
  std::string sqlRequest = "SELECT outputPath, errorPath, owner, status from vsession, job where"
                           " vsession.numsessionid=job.vsession_numsessionid and jobId='"+mjobResult.getJobId()+"'";
  boost::scoped_ptr<DatabaseResult> sqlResult(ServerTMS::getInstance()->getDatabaseVishnu()->getResult(sqlRequest.c_str()));
 
  TMS_Data::Job job;
  JobServer jobServer(msessionServer, mmachineId, job, UNDEFINED);
  std::string acLogin = jobServer.getUserAccountLogin();
  std::cout << "acLogin = " << acLogin << std::endl;

  std::string machineName = jobServer.getMachineName();
  std::cout << "machineName = " << machineName << std::endl;
 
  if (sqlResult->getNbTuples() != 0){ 
    results.clear();
    results = sqlResult->get(0);
    iter = results.begin();
    outputPath = *iter;
    iter++;
    errorPath = *iter;
    iter++;
    owner = *iter;
    if(owner.compare(acLogin)!=0) {
      throw TMSVishnuException(ERRCODE_PERMISSION_DENIED, "You can't get the output of this job because it is for an other owner");
    } 
    iter++;
    status = convertToInt(*iter);
    if(status < 5) {
      throw TMSVishnuException(ERRCODE_JOB_IS_NOT_TERMINATED);
    }
    if(status==6) {
      throw TMSVishnuException(ERRCODE_ALREADY_CANCELED);
    }
  } else {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
  }

  outputPath = outputPath.substr(outputPath.find(":")+1);
  errorPath = errorPath.substr(errorPath.find(":")+1);

  if((outputPath.size()!=0) && errorPath.size()!=0) {
    char* copyOfOutputPath = strdup("/tmp/job_outputPathXXXXXX");
    char* copyOfErrorPath = strdup("/tmp/job_errorPathXXXXXX");

    SSHJobExec().createTmpFile(copyOfOutputPath);
    SSHJobExec().createTmpFile(copyOfErrorPath);

    std::cout << "outputPath = " << outputPath << std::endl;
    std::cout << "errorPath = " << errorPath << std::endl;

    SSHJobExec sshJobExec(NULL, "", "", acLogin, machineName, "", UNDEFINED);
    if(sshJobExec.copyFiles(outputPath, errorPath , copyOfOutputPath, copyOfErrorPath)){
      throw SystemException(ERRCODE_SYSTEM, "SSHJobExec::copyFiles: problem to get the output or error file on this user local account"); 
    }

    mjobResult.setOutputPath(std::string(copyOfOutputPath));
    mjobResult.setErrorPath(std::string(copyOfErrorPath));
  } else {
      throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
  }

  return mjobResult;
}

TMS_Data::ListJobResults_ptr
JobOutputServer::getCompletedJobsOutput() {

   //To check the sessionKey
  msessionServer.check();

  TMS_Data::Job job;
  JobServer jobServer(msessionServer, mmachineId, job, UNDEFINED);
  std::string acLogin = jobServer.getUserAccountLogin();
  std::cout << "acLogin = " << acLogin << std::endl;

  std::string machineName = jobServer.getMachineName();
  std::cout << "machineName = " << machineName << std::endl;

  std::string outputPath;
  std::string errorPath;
  std::string jobId;
  int status;
  std::vector<std::string> results;
  std::vector<std::string>::iterator  iter;

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistJobsResult = ecoreFactory->createListJobResults();

  //To get the output and error path of all jobs
  std::string sqlRequest = "SELECT jobId, outputPath, errorPath, status from vsession, job where"
                           " vsession.numsessionid=job.vsession_numsessionid and owner='"+acLogin+"'";
  boost::scoped_ptr<DatabaseResult> sqlResult(ServerTMS::getInstance()->getDatabaseVishnu()->getResult(sqlRequest.c_str()));

  if (sqlResult->getNbTuples() != 0){
    for (size_t i = 0; i < sqlResult->getNbTuples(); ++i) {
      TMS_Data::JobResult_ptr out = ecoreFactory->createJobResult();
      results.clear();
      results = sqlResult->get(i);
      iter = results.begin();

      jobId = *iter;
      iter++;
      outputPath = *iter;
      iter++;
      errorPath = *iter;
      iter++;
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

        SSHJobExec().createTmpFile(copyOfOutputPath);
        SSHJobExec().createTmpFile(copyOfErrorPath);

        std::cout << "outputPath before = " << outputPath << std::endl;
        std::cout << "errorPath before  = " << errorPath << std::endl;
        std::cout << "jobId = " << jobId << std::endl;

        SSHJobExec sshJobExec(NULL, "", "", acLogin, machineName, "", UNDEFINED);
        if(!sshJobExec.copyFiles(outputPath, errorPath , copyOfOutputPath, copyOfErrorPath)) {;

          out->setJobId(jobId);
          out->setOutputPath(std::string(copyOfOutputPath));
          out->setErrorPath(std::string(copyOfErrorPath));

          mlistJobsResult->getResults().push_back(out);
        }
      }
    }
  }

  return mlistJobsResult;
}

/**
 * \brief Destructor
 */
JobOutputServer::~JobOutputServer() {

}
