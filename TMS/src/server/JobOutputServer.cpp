/**
 * \file JobServer.cpp
 * \brief This file contains the VISHNU JobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#include <boost/format.hpp>
#include "ListJobServer.hpp"
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

  ListJobServer jobLister(mauthKey);
  TMS_Data::ListJobsOptions jobListerOptions;

  jobListerOptions.setJobId(jobId);

  std::string machineId = options->getStringProperty("machineid");
  if (! machineId.empty()) {
    jobListerOptions.setMachineId(machineId);
  }
  // query job list
  boost::scoped_ptr<TMS_Data::ListJobs> jobList( jobLister.list(&jobListerOptions) );

  if (jobList->getNbJobs() == 0) {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID,jobId);
  }

  TMS_Data::Job job = *(jobList->getJobs().at(0));

  if (job.getLocalAccount() != muserSessionInfo.user_aclogin) {
    throw TMSVishnuException(ERRCODE_PERMISSION_DENIED, "You can't get the output of "
                             "this job because it is for an other owner");
  }

  switch(job.getStatus()) {
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

  LOG(boost::str(boost::format("[INFO] Get job ouput: %1%. aclogin: %2%")
                 % job.getId()
                 % job.getLocalAccount()),
      LogInfo);

  mjobResult.setJobId(job.getId());
  mjobResult.setOutputDir(job.getOutputDir());
  mjobResult.setErrorPath( vishnu::removeMachinePrefix( job.getErrorPath() ) );
  mjobResult.setOutputPath( vishnu::removeMachinePrefix( job.getOutputPath() ) );

  return mjobResult;
}

/**
 * \brief Function to get the all completed jobs results
 * \param options Object containing options
 * \return The list of job results data structure
 */
TMS_Data::ListJobResults_ptr
JobOutputServer::getCompletedJobsOutput(JsonObject* options)
{
  ListJobServer jobLister(mauthKey);
  TMS_Data::ListJobsOptions jobListerOptions;

  jobListerOptions.setOwner(muserSessionInfo.userid);

  int days = options->getIntProperty("days");
  if (days <= 0) {
    jobListerOptions.setStatus(vishnu::STATE_COMPLETED);
  } else {
    time_t now = time(NULL);
    jobListerOptions.setFromSubmitDate(now - days * 3600 * 24);
    jobListerOptions.setToSubmitDate(now);
    std::string statuses = (boost::format("%1%%2%")
                            % vishnu::STATE_COMPLETED
                            % vishnu::STATE_DOWNLOADED).str();
    jobListerOptions.setMultipleStatus(statuses);
  }

  // query job list
  boost::scoped_ptr<TMS_Data::ListJobs> jobList( jobLister.list( &jobListerOptions ) );

  if (jobList->getNbJobs() == 0) {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID, "No job available to download");
  }

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistJobsResult = ecoreFactory->createListJobResults();

  for (size_t index = 0; index < jobList->getNbJobs(); ++index) {

    TMS_Data::Job_ptr job = jobList->getJobs().get(index);
    TMS_Data::JobResult_ptr jobResult = new TMS_Data::JobResult();
    jobResult->setJobId(job->getId());
    jobResult->setOutputDir(job->getOutputDir());
    jobResult->setErrorPath( vishnu::removeMachinePrefix(job->getErrorPath()) );
    jobResult->setOutputPath( vishnu::removeMachinePrefix(job->getOutputPath()) );
    mlistJobsResult->getResults().push_back(jobResult);

    // Mark the job as downloaded, so it will be ignored at the subsequent calls
    std::string query = boost::str(
                          boost::format("UPDATE job SET status=%1% "
                                        " WHERE job.id='%2%';"
                                        ) % vishnu::convertToString(vishnu::STATE_DOWNLOADED)
                          % mdatabaseInstance->escapeData(jobResult->getJobId()));

    mdatabaseInstance->process(query);

    LOG(boost::str(boost::format("[INFO] request to job ouput: %1%. aclogin: %2%")
                   % jobResult->getJobId()
                   % muserSessionInfo.user_aclogin),
        LogInfo);
  }

  mlistJobsResult->setNbJobs( jobList->getNbJobs() );

  return mlistJobsResult;
}

/**
 * \brief Destructor
 */
JobOutputServer::~JobOutputServer() {

}
