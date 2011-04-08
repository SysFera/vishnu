/**
 * \file api_tms.hpp
 * \brief This file contains the VISHNU api functions for TMS package.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date March 2011
 */

#ifndef _API_TMS_H
#define _API_TMS_H

#include <iostream>
#include <string>

//UMS include
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"
#include "LocalAccountProxy.hpp"
#include "ConfigurationProxy.hpp"
#include "OptionValueProxy.hpp"
#include "QueryProxy.hpp"
#include "UtilsProxy.hpp"
#include "UserException.hpp"
#include "utilVishnu.hpp"
#include "utilsTMSClient.hpp"

//TMS include
#include "JobProxy.hpp"
#include "TMSMachineProxy.hpp"
#include "JobOutPutProxy.hpp"

using namespace TMS_Data;

namespace vishnu {
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
  throw(UserException, SystemException);

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
            const std::string& jobId)
  throw(UserException, SystemException);

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
  throw(UserException, SystemException);

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
  throw(UserException, SystemException);

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
  throw(UserException, SystemException);

  /**
  * \brief The listQueues function gets queues information
  * \fn int listQueues(const std::string& sessionKey, const std::string& machineId, ListQueues& listofQueues)
  * \param sessionKey : The session key
  * \param machineId : The id of the machine
  * \param listofQueues : The list of queues
  * \return int : an error code
  */
  int
  listQueues(const std::string& sessionKey,
             const std::string& machineId,
             ListQueues& listofQueues)
  throw(UserException, SystemException);

  /**
  * \brief The setMachineRefreshPeriod function sets the refresh period of output and error files contents
  * \fn int setMachineRefreshPeriod(const std::string& sessionKey, const std::string& machineId, const int& value)
  * \param sessionKey : The session key
  * \param machineId : The id of the machine
  * \param value : Is the refresh interval value (in seconds)
  * \return int : an error code
  */
  int
  setMachineRefreshPeriod(const std::string& sessionKey,
                          const std::string& machineId,
                          const int& value)
  throw(UserException, SystemException);

  /**
  * \brief The getMachineRefreshPeriod function gets the refresh period of output and error files contents
  * \fn int getMachineRefreshPeriod(const std::string& sessionKey, const std::string& machineId)
  * \param sessionKey : The session key
  * \param machineId : Represents the machine id
  * \param value The refresh period value
  * \return the value of the refresh period
  */
  int
  getMachineRefreshPeriod(const std::string& sessionKey,
                          const std::string& machineId,
                          int& value
                         )
  throw(UserException, SystemException);

  /**
  * \brief The getJobOutput function gets outputPath and errorPath of a job from its id
  * \fn int getJobOutput(const std::string& sessionKey, const std::string& machineId, const std::string& jobId, JobResult& outputInfo, std::string& infoMsg)
  * \param sessionKey : The session key
  * \param machineId : The id of the machine
  * \param jobId : The Id of the job
  * \param outputInfo : The  Job object  containing the job output information (ex: outputPath and errorPath) of the job to submit
  * \param infoMsg : The information message
  * \return int : an error code
  */
  int
  getJobOutput(const std::string& sessionKey,
               const std::string& machineId,
               const std::string& jobId,
               JobResult_ptr& outputInfos)
  throw(UserException, SystemException);

  /**
  * \brief The getAllJobsOutput function dynamically gets outputPath and errorPath of completed jobs
  * \fn int getAllJobsOutput(const std::string& sessionKey, const std::string& machineId, ListJobResults& listOfResults)
  * \param sessionKey : The session key
  * \param machineId : The id of the machine
  * \param listOfResults : Is the list of jobs results
  * \return int : an error code
  */
  int
  getAllJobsOutput(const std::string& sessionKey,
                   const std::string& machineId,
                   ListJobResults& listOfResults)
  throw(UserException, SystemException);

}//END namespace vishnu

#endif
