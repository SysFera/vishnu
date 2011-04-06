#ifndef TMS_TORQUE_SERVER_H
#define TMS_TORQUE_SERVER_H

extern "C" {
#include "pbs_ifl.h" //Torque includes
#include "pbs_error.h"
#include "set_attr.h"
#include "cnt2server.h" 
}
#include "BatchServer.hpp"

class TorqueServer : public BatchServer
{

  public:

  TorqueServer();
  int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job);
  int cancel() { };
  TMS_Data::Job getJob() { };
  TMS_Data::ListJobs* listJobs() { };
  TMS_Data::JobResult getJobResults() { };
  TMS_Data::ListQueues* listQueues() { };
  TMS_Data::ListJobResults* getAllJobsResults() { };

  ~TorqueServer() { }
  private:
  char* mserverName;
};

#endif
