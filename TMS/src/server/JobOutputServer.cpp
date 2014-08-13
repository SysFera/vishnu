/**
 * \file JobServer.cpp
 * \brief This file contains the VISHNU JobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#include <boost/format.hpp>
#include "JobOutputServer.hpp"
#include "TMSVishnuException.hpp"
#include "LocalAccountServer.hpp"
#include "SSHJobExec.hpp"
#include "utilServer.hpp"
#include "DbFactory.hpp"
#include "Logger.hpp"

/**
 * \brief Constructor
 * \param authKey The session info
 * \param machineId The machine identifier
 */
JobOutputServer::JobOutputServer(const std::string& authKey,
                                 const std::string& machineId)
  : mauthKey(authKey), mmachineId(machineId), mlistJobsResult(NULL)
{
  DbFactory factory;
  mdatabaseInstance = factory.getDatabaseInstance();

  vishnu::validateAuthKey(mauthKey, mmachineId, mdatabaseInstance, muserSessionInfo);
}


/**
 * \brief Function to get the job results
 * \param options Object containing options
 * \param jobId The job identifier
 * \return The job results data structure
 */
TMS_Data::JobResult
JobOutputServer::getJobOutput(JsonObject* options, const std::string& jobId) {

  //To get the output and error path of the job
  std::string query;

  std::string machineId = options->getStringProperty("machineid");
  if (! machineId.empty()){
    query = boost::str(
              boost::format("SELECT outputPath, errorPath, owner, job.status, outputDir "
                            " FROM job, machine"
                            " WHERE job.id='%1%'"
                            "   AND machine.machineid='%2%'"
                            "   AND machine.nummachineid=job.machine_nummachineid;"
                            )
              % mdatabaseInstance->escapeData(jobId)
              % machineId);
  } else {
    query = boost::str(
              boost::format("SELECT outputPath, errorPath, owner, status, outputDir"
                            " FROM job"
                            " WHERE job.jobId='%1%';"
                            )
              % mdatabaseInstance->escapeData(jobId));
  }

  boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseInstance->getResult(query));
  if(sqlResult->getNbTuples() == 0) {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
  }

  std::vector<std::string> results = sqlResult->get(0);
  std::vector<std::string>::iterator iter = results.begin();
  std::string outputPath = *iter++;
  std::string errorPath = *iter++;
  std::string owner = *iter++;
  int status = vishnu::convertToInt( *iter++ );
  std::string outputDir = *iter++;

  if (owner != muserSessionInfo.user_aclogin) {
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
      throw TMSVishnuException(ERRCODE_JOB_IS_NOT_TERMINATED);
      break;
  }

  outputPath = outputPath.substr(outputPath.find(":")+1);
  errorPath = errorPath.substr(errorPath.find(":")+1);

  mjobResult.setOutputDir(outputDir) ;
  mjobResult.setOutputPath(outputPath) ;
  mjobResult.setErrorPath(errorPath) ;
  LOG(boost::str(boost::format("[INFO] request to job ouput: %1%. aclogin: %2%")
                 % jobId
                 % owner), LogInfo);

  return mjobResult;
}

/**
 * \brief Function to get the all completed jobs results
 * \param options Object containing options
 * \return The list of job results data structure
 */
TMS_Data::ListJobResults_ptr
JobOutputServer::getCompletedJobsOutput(JsonObject* options) {

  int days = options->getIntProperty("days");

  std::vector<std::string> results;
  std::vector<std::string>::iterator iter;

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistJobsResult = ecoreFactory->createListJobResults();

  //To get the output and error path of all jobs
  std::string sqlQuery;
  if (days <= 0) {
    // Here download only newly completed jobs
    sqlQuery = boost::str(
                 boost::format("SELECT job.id, outputPath, errorPath, outputDir"
                               " FROM vsession, job"
                               " WHERE vsession.numsessionid=job.vsession_numsessionid"
                               "  AND job.users_numuserid=%1%"
                               "  AND job.machine_nummachineid=%3%"
                               "  AND job.status=%2%;"
                               )
                 % muserSessionInfo.num_user
                 % muserSessionInfo.num_machine
                 % vishnu::STATE_COMPLETED);
  } else {
    // Here also download jobs already downloaded
    sqlQuery =  boost::str
                (boost::format("SELECT job.id, outputPath, errorPath, outputDir"
                               " FROM vsession, job"
                               " WHERE vsession.numsessionid=job.vsession_numsessionid"
                               "  AND job.users_numuserid=%1%"
                               "  AND job.machine_nummachineid=%2%"
                               "  AND (job.status=%3% OR job.status=%4%)"
                               "  AND submitdate >= DATE_SUB(CURDATE(),INTERVAL %5% DAY);"
                               )
                 % muserSessionInfo.num_user
                 % muserSessionInfo.num_machine
                 % vishnu::STATE_COMPLETED
                 % vishnu::STATE_DOWNLOADED
                 % days);
  }

  boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseInstance->getResult(sqlQuery.c_str()));

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
                                       " WHERE job.id='%2%';"
                                       ) % vishnu::convertToString(vishnu::STATE_DOWNLOADED)
                         % mdatabaseInstance->escapeData(jobId)).str();
    mdatabaseInstance->process(query);
    LOG(boost::str(boost::format("[INFO] request to job ouput: %1%. aclogin: %2%")
                   % jobId
                   % muserSessionInfo.user_aclogin), LogInfo);
  }
  mlistJobsResult->setNbJobs(mlistJobsResult->getResults().size());

  return mlistJobsResult;
}

/**
 * \brief Destructor
 */
JobOutputServer::~JobOutputServer() {

}
