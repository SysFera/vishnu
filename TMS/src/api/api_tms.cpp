/**
 * \file api_tms.cpp
 * \brief This file contains the VISHNU api functions for TMS package.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date March 2011
 */

#include "api_tms.hpp"

/**
  * \brief The submitJob function submits job on a machine through the use of a script (scriptFilePath).
  * \fn int submitJob(const std::string& sessionKey, const std::string& machineId, const std::string& scriptFilePath, Job& jobInfo, const SubmitOptions& options)
  * \param sessionKey : The session key
  * \param machineId : Is the id of the machine on which the job must be submitted
  * \param scriptFilePath : The path to the file containing the characteristics (job command, and batch scheduler directive required or optional) of the job to submit.
  * \param jobInfo : The  Job object containing the output information (ex: jobId and jobPath) of the job to submit
  * \param options : Is an instance of the class SubmitOptions. Each optionnal value is associated to a set operation (e.g: setNbCpu(...)) in the class SubmitOptions.  If no set operation is not called on the instance object  options, the job is submitted with the options defined in the scriptFilePath. Otherewise the job is submitted with the optionnal values set by the options object and optionnal values defined in the scriptFilePath, but optionnal values set by SubmitOptions object take precedence over those in scriptFilePath. With in the object options or within the scriptFilePath, the last occurance of an optionnal value takes precedence over earlier occurance.
  * \return int : an error code
  */
int
submitJob(const std::string& sessionKey,
          const std::string& machineId,
          const std::string& scriptFilePath,
          Job& jobInfo,
          const SubmitOptions& options)
throw(UserException, SystemException) {

  jobInfo.setJobPath(scriptFilePath);
  SessionProxy sessionProxy(sessionKey);

  UMS_Data::Machine machine;
  machine.setMachineId(machineId);

  JobProxy jobProxy(sessionProxy,
                    MachineProxy(machine, sessionProxy),
                    jobInfo);

  int ret = jobProxy.submitJob(options);
  jobInfo = jobProxy.getData();

  return ret;
}

/**
* \brief The cancelJob function cancels a job from its id
* \fn int cancelJob(const std::string& sessionKey, const std::string& machineId, const std::string& jobId, std::string& infoMsg)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param jobId : The Id of the job
* \param infoMsg : The information message
* \return int : an error code
*/
int
cancelJob(const std::string& sessionKey,
          const std::string& machineId,
          const std::string& jobId,
          std::string& infoMsg)
throw(UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);

  TMS_Data::Job job;
  job.setJobId(jobId);

  UMS_Data::Machine machine;
  machine.setMachineId(machineId);

  return JobProxy(sessionProxy,
                  MachineProxy(machine, sessionProxy),
                  job).cancelJob();

}


/**
* \brief The getJobInfo function gets information on a job from its id
* \fn int getJobInfo(const std::string& sessionKey, const std::string& machineId, const std::string& jobId, Job& jobInfos)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param jobId : The id of the job
* \param jobInfos : The resulting information on the job
* \return int : an error code
*/

int
getJobInfo(const std::string& sessionKey,
            const std::string& machineId,
            const std::string& jobId,
            Job_ptr& jobInfos)
throw(UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);

  UMS_Data::Machine machine;
  machine.setMachineId(machineId);
  MachineProxy machineProxy(machine, sessionProxy);

  std::string serviceName = "getListOfJobs_";
  serviceName.append(machineProxy.getData().getMachineId());

  TMS_Data::ListJobsOptions listJobsOptions;
  listJobsOptions.setJobId(jobId);

  QueryProxy<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>
  query(listJobsOptions, sessionProxy, serviceName, machineProxy.getData().getMachineId());

  TMS_Data::ListJobs* listJobs_ptr = query.list();

  if(listJobs_ptr != NULL) {
    jobInfos = listJobs_ptr->getJobs().get(0);
  }
  delete listJobs_ptr;
  return 0;
}

/**
* \brief The listJobs function gets a list of all submitted jobs
* \fn int listJobs(const std::string& sessionKey, const std::string& machineId, ListJobs& listOfJobs, const ListJobsOptions& options)
* \param sessionKey : The session key
* \param machineId : The id of the machine
* \param listOfJobs : The constructed object list of jobs
* \param options : Additional options for jobs listing
* \return int : an error code
*/
int
listJobs(const std::string& sessionKey,
          const std::string& machineId,
          ListJobs& listOfJobs,
          const ListJobsOptions& options)
throw(UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);

  UMS_Data::Machine machine;
  machine.setMachineId(machineId);
  MachineProxy machineProxy(machine, sessionProxy);

  std::string serviceName = "getListOfJobs_";
  serviceName.append(machineProxy.getData().getMachineId());

  QueryProxy<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>
  query(options, sessionProxy, serviceName, machineProxy.getData().getMachineId());

  TMS_Data::ListJobs* listJobs_ptr = query.list();

  if(listJobs_ptr != NULL) {
    TMS_Data::Job_ptr job;
    for(unsigned int i = 0; i < listJobs_ptr->getJobs().size(); i++) {
      job = listJobs_ptr->getJobs().get(i);
      listOfJobs.getJobs().push_back(job);
    }
  }
  delete listJobs_ptr;
  return 0;
}

/**
* \brief The getJobProgress function gets the progression status of jobs
* \fn int getJobProgress(const std::string& sessionKey, const std::string& machineId, Progression& progress, const ProgressOptions& options)
* \param sessionKey : The session key
* \param machineId : Is the id of the machine to get the jobs progression.
* \param listOfProgress : Is the object containing jobs progression information
* \param options : Is an object containing the available options jobs for progression .
* \return int : an error code
*/
int
getJobProgress(const std::string& sessionKey,
              const std::string& machineId,
              ListProgression& listOfProgress,
              const ProgressOptions& options)
throw(UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);

  UMS_Data::Machine machine;
  machine.setMachineId(machineId);
  MachineProxy machineProxy(machine, sessionProxy);

  std::string serviceName = "getJobsProgression_";
  serviceName.append(machineProxy.getData().getMachineId());

  QueryProxy<TMS_Data::ProgressOptions, TMS_Data::ListProgression>
  query(options, sessionProxy, serviceName, machineProxy.getData().getMachineId());

  TMS_Data::ListProgression* listProgression_ptr = query.list();

  if(listProgression_ptr != NULL) {
    TMS_Data::Progression_ptr progression;
    for(unsigned int i = 0; i < listProgression_ptr->getProgress().size(); i++) {
      progression = listProgression_ptr->getProgress().get(i);
      listOfProgress.getProgress().push_back(progression);
    }
  }
  delete listProgression_ptr;
  return 0;
}