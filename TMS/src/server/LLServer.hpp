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

  LLServer();
  int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp=NULL);
  int cancel(const char* jobId) { };
  TMS_Data::Job getJob() { };
  TMS_Data::ListJobs* listJobs(ListJobs& jobs, ListJobsOptions op) { };
  TMS_Data::JobResult getJobResults() { };
  TMS_Data::ListQueues* listQueues() { };
  TMS_Data::ListJobResults* getAllJobsResults() { };
  int getJobProgress(){};

  ~LLServer() { }
  private:
  char* mserverName;

  
};

#endif
