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

  SessionProxy sessionProxy(sessionKey);
  JobProxy jobProxy(sessionProxy, MachineProxy(machineId, sessionProxy));

  int ret = jobProxy.submitJob(options);
  jobInfo = jobProxy.getData();

  return ret;
}

/**
* \brief The cancelJob function cancels a job from its id
* \fn int cancelJob(const std::string& sessionKey, const std::string& machineId, const std::string& jobId, std::string& infoMsg)
* \param sessionKey : The session key
* \param machineId : Machine hash key
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


  return JobProxy(sessionProxy,
                  MachineProxy(machineId, sessionProxy),
                  job).cancelJob();

}
