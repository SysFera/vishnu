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
  int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job);
  int cancel() { };
  TMS_Data::Job getJob() { };
  TMS_Data::ListJobs* listJobs() { };
  TMS_Data::JobResult getJobResults() { };
  TMS_Data::ListQueues* listQueues() { };
  TMS_Data::ListJobResults* getAllJobsResults() { };

  ~LLServer() { }
  private:
  char* mserverName;

  
};

#endif
