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
    int cancel(const char* jobId) { };
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

    TMS_Data::ListProgression* getJobProgress(TMS_Data::ProgressOptions) {};

    ~LLServer() { }

  TMS_Data::ListJobs_ptr mjobs;
};

#endif
