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
 * \param jobResult The job result data structure
 * \brief Constructor
 */
JobOutputServer::JobOutputServer(const SessionServer& sessionServer,
    const std::string& machineId,
    const TMS_Data::JobResult& jobResult):
  msessionServer(sessionServer), mmachineId(machineId), mjobResult(jobResult)
{
}
/**
 * \brief Function to get the job results
 * \param jobId The Id of the
 * \return The job results data structure
 */
TMS_Data::JobResult
JobOutputServer::getJobOutput() {

  //Recuperer les output depuis la base de donnees
  std::string outputPath;// = JobServer::mapOfOutputPath[mjobResult.getJobId()];
  std::string errorPath;// = JobServer::mapOfErrorPath[mjobResult.getJobId()];

  std::cout << "outputPath before = " << outputPath << std::endl;
  std::cout << "errorPath before = " << errorPath << std::endl;

  outputPath = outputPath.substr(outputPath.find(":")+1);
  errorPath = errorPath.substr(errorPath.find(":")+1);

  if((outputPath.size()!=0) && errorPath.size()!=0) {
    char* copyOfOutputPath = strdup("/tmp/job_outputPathXXXXXX");
    char* copyOfErrorPath = strdup("/tmp/job_errorPathXXXXXX");

    SSHJobExec().createTmpFile(copyOfOutputPath);
    SSHJobExec().createTmpFile(copyOfErrorPath);


    TMS_Data::Job job;
    JobServer jobServer(msessionServer, mmachineId, job, UNDEFINED);
    std::string acLogin = jobServer.getUserAccountLogin();
    std::cout << "acLogin = " << acLogin << std::endl;

    std::string machineName = jobServer.getMachineName();
    std::cout << "machineName = " << machineName << std::endl;

    std::cout << "outputPath = " << outputPath << std::endl;
    std::cout << "errorPath = " << errorPath << std::endl;

    SSHJobExec sshJobExec(NULL, "", "", acLogin, machineName, "", UNDEFINED);
    sshJobExec.copyFiles(outputPath, errorPath , copyOfOutputPath, copyOfErrorPath);

    mjobResult.setOutputPath(std::string(copyOfOutputPath));
    mjobResult.setErrorPath(std::string(copyOfErrorPath));
  } else {
      throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
  }

  return mjobResult;
}

/**
 * \brief Destructor
 */
JobOutputServer::~JobOutputServer() {

}
