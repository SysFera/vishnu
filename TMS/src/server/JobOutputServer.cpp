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
#include <boost/format.hpp>

using namespace vishnu;


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
  mjobResult(jobResult), msessionServer(sessionServer), mmachineId(machineId)
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

  msessionServer.check();  // check the sessionKey

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
                           "  AND job.jobId='"+mdatabaseVishnu->escapeData(mjobResult.getJobId())+"' "
                           "  AND job.submitMachineId='"+mdatabaseVishnu->escapeData(mmachineId)+"'" ;

  boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseVishnu->getResult(sqlRequest.c_str()));
  if(sqlResult->getNbTuples() == 0) {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
  }

  results.clear();
  results = sqlResult->get(0);

  iter = results.begin();
  outputPath = *iter++;
  errorPath = *iter++;
  owner = *iter++;
  status = convertToInt( *iter++ );
  subDateStr = *iter++;
  outputDir = *iter++;

  std::string acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);
  if (owner != acLogin) {
    throw TMSVishnuException(ERRCODE_PERMISSION_DENIED, "You can't get the output of "
                             "this job because it is for an other owner");
  }

  switch(status) {
  case vishnu::STATE_COMPLETED:
  case vishnu::STATE_DOWNLOADED:
    break;
  case vishnu::STATE_CANCELLED:
  case vishnu::STATE_FAILED:
    throw TMSVishnuException(ERRCODE_INVALID_PARAM, "Can't get output from cancelled or failed jobs");
    break;
  case vishnu::STATE_UNDEFINED:
  case vishnu::STATE_SUBMITTED:
  case vishnu::STATE_QUEUED:
  case vishnu::STATE_WAITING:
  case vishnu::STATE_RUNNING:
  default:
    throw TMSVishnuException(ERRCODE_JOB_IS_NOT_TERMINATED, "The job is not completed or is in a wrong state");
    break;
  }

  outputPath = outputPath.substr(outputPath.find(":")+1);
  errorPath = errorPath.substr(errorPath.find(":")+1);

  if(outputPath.empty() || errorPath.empty()) {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
  }

  mjobResult.setOutputDir(outputDir) ;
  mjobResult.setOutputPath(outputPath) ;
  mjobResult.setErrorPath(errorPath) ;
  LOG(boost::format("[INFO] Request to job ouput: %1%. aclogin: %2%")% mjobResult.getJobId() % owner, 1);

  return mjobResult;
}

/**
 * \brief Function to get the all completed jobs results
 * \param options Object containing options
 * \return The list of job results data structure
 */
TMS_Data::ListJobResults_ptr
JobOutputServer::getCompletedJobsOutput(const TMS_Data::JobOuputOptions& options) {

  msessionServer.check(); // check the sessionKey

  std::string acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);
  std::vector<std::string> results;
  std::vector<std::string>::iterator iter;

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistJobsResult = ecoreFactory->createListJobResults();

  //To get the output and error path of all jobs
  std::string sqlQuery;
  if (options.getDays() <= 0) {
    // Here download only newly completed jobs
    sqlQuery = (boost::format("SELECT jobId, outputPath, errorPath, outputDir "
                              "FROM vsession, job "
                              "WHERE vsession.numsessionid=job.vsession_numsessionid"
                              "  AND job.owner='%1%'"
                              "  AND job.submitMachineId='%2%'"
                              "  AND job.status=%3%"
                              ) % mdatabaseVishnu->escapeData(acLogin)
                % mdatabaseVishnu->escapeData(mmachineId)
                % vishnu::convertToString(vishnu::STATE_COMPLETED)).str();
  } else {
    // Here also download jobs already downloaded
    sqlQuery = (boost::format("SELECT jobId, outputPath, errorPath, outputDir "
                              "FROM vsession, job "
                              "WHERE vsession.numsessionid=job.vsession_numsessionid"
                              "  AND submitdate >= DATE_SUB(CURDATE(),INTERVAL %1% DAY)"
                              "  AND job.owner='%2%'"
                              "  AND job.submitMachineId='%3%'"
                              "  AND (job.status=%4% OR job.status=%5%)"
                              )
                % vishnu::convertToString(options.getDays())
                % mdatabaseVishnu->escapeData(acLogin)
                % mdatabaseVishnu->escapeData(mmachineId)
                % vishnu::convertToString(vishnu::STATE_COMPLETED)
                % vishnu::convertToString(vishnu::STATE_DOWNLOADED)).str();
  }

  boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseVishnu->getResult(sqlQuery.c_str()));

  if (sqlResult->getNbTuples() == 0) {
    return mlistJobsResult;
  }

  for (size_t i = 0; i < sqlResult->getNbTuples(); ++i) {
    results.clear();
    results = sqlResult->get(i);
    iter = results.begin();

    std::string jobId = *iter;
    ++iter;
    std::string outputPath = *iter;
    ++iter;
    std::string errorPath = *iter;
    ++iter;
    std::string outputDir = *iter;

    // remove the hostname on the paths
    size_t pos1 = outputPath.find(":");
    if (pos1 != std::string::npos) {
      outputPath = outputPath.substr(pos1+1);
    }

    size_t pos2 = errorPath.find(":");
    if (pos2 != std::string::npos) {
      errorPath = errorPath.substr(pos2+1);
    }

    TMS_Data::JobResult_ptr curResult = ecoreFactory->createJobResult();
    curResult->setJobId(jobId);
    curResult->setOutputDir( outputDir ) ;
    curResult->setOutputPath(outputPath) ;
    curResult->setErrorPath( errorPath) ;
    mlistJobsResult->getResults().push_back(curResult);

    // Mark the job as downloaded, so it will be ignored at the subsequent calls
    std::string query = (boost::format("UPDATE job SET status=%1% "
                                       " WHERE jobId='%2%';"
                                       ) % vishnu::convertToString(vishnu::STATE_DOWNLOADED)
                         % mdatabaseVishnu->escapeData(jobId)).str();
    mdatabaseVishnu->process(query);
    LOG(boost::format("[INFO] Request to job ouput: %1%. aclogin: %2%")% jobId % acLogin, 1);
  }
  mlistJobsResult->setNbJobs(mlistJobsResult->getResults().size());

  return mlistJobsResult;
}

/**
 * \brief Destructor
 */
JobOutputServer::~JobOutputServer() {

}
