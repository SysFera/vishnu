#ifndef TMS_LL_SERVER_H
#define TMS_LL_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "BatchServer.hpp"

class LLServer : public BatchServer
{

public:
  LLServer();
  int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp=NULL);
  int cancel(const char* jobId);
  TMS_Data::Job getJob() { };
  /**
   * \brief To list the jobs on the torque batch
   * \fn TMS_Data::ListJobs* listJobs(ListJobsOptions op)
   * \param op: Options to list the jobs
   * \return The list of the jobs
   */
  TMS_Data::ListJobs*
  listJobs(TMS_Data::ListJobsOptions op);
  TMS_Data::JobResult getJobResults() { };
  TMS_Data::ListQueues* listQueues() { };
  TMS_Data::ListJobResults* getAllJobsResults() { };
  /**
   * \brief To get the info about a job
   * \fn TMS_Data::Job* getJobInfo(string job)
   * \param job: The id of the job
   * \return The filled job
   */
  TMS_Data::Job* 
  getJobInfo(string job);

  TMS_Data::ListProgression* getJobProgress(TMS_Data::ProgressOptions);

  ~LLServer() { }

private:
  int remove_test(TMS_Data::Job& job);

  /**
   * \brief To list the jobs on the torque batch
   * \fn TMS_Data::ListJobs* listJobs(ListJobsOptions op)
   * \param op: Options to list the jobs
   * \return The list of the jobs
   */
  TMS_Data::ListJobs*
  listAllJobs();

  /**
   * \brief Check if the job in the iterator corresponds to the option
   * \fn bool notIn(TMS_Data::Jobs it, TMS_Data::ListJobsOptions op)
   * \param it: Iterator on the job
   * \param op: The options to use as a filter
   * \return If the job pointed by the iterator does not fit the options (to remove from list if true returned)
   */
  bool 
  notIn(TMS_Data::Job_ptr it, TMS_Data::ListJobsOptions op);

  /**
   * \brief To extract the job corresponding to the jobid from the member job list
   * \fn TMS_Data::Job* filterJob(string jobid)
   * \param jobid: The id of the job searched
   * \return The corresponding job, throw an exception if it is not found
   */
  TMS_Data::Job*
  filterJob(string jobid);
  
  /**
   * \brief Filter the jobs in the member list mjobs, filters are based on the options op
   * \fn TMS_Data::ListJobs* filterJobs(TMS_Data::ListJobsOptions op)
   * \param op: The option to use as a filter on the list
   * \return The filtered mjobs list
   */
  TMS_Data::ListJobs*
  filterJobs(TMS_Data::ListJobsOptions op);
  /**
   * \brief To transform a job in a progression
   * \fn void LLServer::makeProg(TMS_Data::Progression_ptr prog, TMS_Data::Job_ptr job)
   * \param prog: OUT, progression to fill
   * \param job: IN, job source to convert in progression
   */
  void
  makeProg(TMS_Data::Progression_ptr prog, TMS_Data::Job_ptr job);
  /**
   * \brief List of jobs
   */  
  TMS_Data::ListJobs_ptr mjobs;
  /**
   * \brief List of jobs
   */  
  TMS_Data::ListProgression_ptr mprog;
  /**
   * \brief A job
   */  
  TMS_Data::Job_ptr mjob;
};

#endif
