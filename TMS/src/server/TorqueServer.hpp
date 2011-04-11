#ifndef TMS_TORQUE_SERVER_H
#define TMS_TORQUE_SERVER_H

#include "BatchServer.hpp"

class TorqueServer : public BatchServer
{

  public:

  TorqueServer();
  int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp=NULL);
  int cancel(const char* jobId) ;
  TMS_Data::Job getJob() { };
  TMS_Data::ListJobs* listJobs() { };
  TMS_Data::JobResult getJobResults() { };
  TMS_Data::ListQueues* listQueues() { };
  TMS_Data::ListJobResults* getAllJobsResults() { };

  ~TorqueServer(); 

  private:
  int pbs_cancel(const char* jobId, char remoteServer[], bool isLocal=true);
  void process_options(const TMS_Data::SubmitOptions& options, std::vector<std::string>&cmds_options);
};

#endif
