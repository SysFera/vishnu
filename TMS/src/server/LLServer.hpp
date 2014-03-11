/**
 * \file LLServer.hpp
 * \brief This file contains the VISHNU LLServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#ifndef TMS_LL_SERVER_H
#define TMS_LL_SERVER_H

#include <string>

#include "BatchServer.hpp"

/**
 * \class LLServer
 * \brief LLServer class implementation
 */
class LLServer : public BatchServer
{

public:

  /**
   * \brief Constructor
   */
  LLServer();

  /**
   * \brief Function to submit LL job
   * \param scriptPath the path to the script containing the job characteristique
   * \param options the options to submit job
   * \param jobSteps The list of job steps
   * \param envp The list of environment variables used by LL submission function
   * \return raises an exception on error
   */
  int
  submit(const std::string& scriptPath,
         const TMS_Data::SubmitOptions& options,
         std::vector<TMS_Data::Job>& jobSteps,
         char** envp=NULL);

  /**
   * \brief Function to cancel job
   * \param jobId the identifier of the job to cancel
   * \return raises an exception on error
   */
  int
  cancel(const std::string& jobId);

  /**
   * \brief Function to get the status of the job
   * \param jobId the identifier of the job
   * \return -1 if the job is unknown or server not  unavailable
   */
  int
  getJobState(const std::string& jobId);

  /**
   * \brief Function to get the start time of the job
   * \param jobId the identifier of the job
   * \return 0 if the job is unknown or server not  unavailable
   */
  time_t
  getJobStartTime(const std::string& jobId);

  /**
   * \brief Function to request the status of queues
   * \param optQueueName (optional) the name of the queue to request
   * \return The requested status in to ListQueues data structure
   */
  TMS_Data::ListQueues*
  listQueues(const std::string& optQueueName=std::string());

  /**
   * \brief Function to get a list of submitted jobs
   * \param listOfJobs the ListJobs structure to fill
   * \param ignoredIds the list of job ids to ignore
   */
  void fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                      const std::vector<std::string>& ignoredIds=std::vector<std::string>());

  /**
   * \brief Destructor
   */
  ~LLServer();

private:

  /**
   * \brief Function to insert option into string
   * \param optionLineToInsert the option to insert
   * \param content The buffer containing the inserted option
   * \return raises an exception on error
   */
  void
  insertOptionLine(const std::string& optionLineToInsert,
                       std::string& content);

  /**
   * \brief Function to treat the submission options
   * \param scriptPath The job script path
   * \param options the object which contains the SubmitOptions options values
   * \return raises an exception on error
   */
  void
  processOptions(const char* scriptPath,
                 const TMS_Data::SubmitOptions& options);

  /**
   * \brief Function to convert the LL state into VISHNU state
   * \param state the state to convert
   * \return VISHNU state
   */
  int
  convertLLStateToVishnuState(int state);

  /**
   * \brief Function to convert the LL priority into VISHNU priority
   * \param prio the priority to convert
   * \return VISHNU state
   */
  int
  convertLLPrioToVishnuPrio(const int& prio);

  /**
   * \brief Function to compute the number of running and waiting jobs of each queue
   * \param run contains the number of running jobs of each queue
   * \param que contains the number of waiting jobs of each queue
   * \return raises an exception on error
   */
  int
  computeNbRunJobsAndQueueJobs(std::map<std::string, size_t>& run,
                               std::map<std::string, size_t>& que);
  /**
   * \brief Function to get the number of nodes of the job
   * \param jobId the identifier of the job
   * \return -1 if the job is unknown or server not  unavailable
   */
  int
  getJobNodeCount(const std::string& jobId);

  /**
   * \brief Function to get the value of LoadLeveler resource (nodes, walltime, cpu) in the script
   * \param file The file contain the script to scan
   * \param optionLetterSyntax The syntax of the option (nodes, walltime, cpu) whose value will be returned
   * \return The value of the resource
   */
  std::string
  getLLResourceValue(const char* file,
                     const std::string& optionLetterSyntax);

  /**
   * \brief Function to replace some environment varia*bles in a string
   * \param scriptpath The script path to modify
   */
  void replaceEnvVariables(const char* scriptPath);

  /**
   * \brief ListQueues returned
   */
  TMS_Data::ListQueues_ptr mlistQueues;
};

#endif
