#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include <iostream>
#include <unistd.h>

#include "PosixServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"

#include "TmsPosixClient.hpp"

PosixServer::PosixServer():BatchServer(){
}


int
PosixServer::submit(const char* scriptPath,
                    const TMS_Data::SubmitOptions& options,
                    TMS_Data::Job& job,
                    char** envp){
  int ret;
  struct st_job resultat;
  struct st_submit op;
  memcpy(op.name, options.getName().c_str(), strlen(options.getName().c_str()));
  op.walltime = options.getWallTime();
  memcpy(op.OutPutPath, options.getOutputPath().c_str(), strlen(options.getOutputPath().c_str()));
  memcpy(op.ErrorPath, options.getErrorPath().c_str(), strlen(options.getErrorPath().c_str()));
  memcpy(op.WorkDir, options.getWorkingDir().c_str(), strlen(options.getWorkingDir().c_str()));

  buildEnvironment();

  switch(fork()) {
    case -1:
      return -1;
    case 0:
  LaunchDaemon();
      break;
    default:
      break;
  }

  std::cout<<"Submit"<<std::endl;
  ret = ReqSubmit(scriptPath, &resultat, &op);
  job.setBatchJobId(std::string(resultat.JobId));
  return ret;
}


int
PosixServer::cancel(const char* jobId){
  return ReqCancel(jobId);
}


int
PosixServer::getJobState(const std::string& jobId){
  struct st_job resultat;
  int ret;

  ret = ReqInfo(jobId.c_str(), &resultat);
  // A voir translation des codes erreurs ou correspondance ....
  return resultat.state;
//  return 5;
}


time_t
PosixServer::getJobStartTime(const std::string& jobId){
  struct st_job resultat;
  int ret;

  ret = ReqInfo(jobId.c_str(), &resultat);
  return resultat.startTime;
//  return time_t();
}


TMS_Data::ListQueues*
PosixServer::listQueues(const std::string& optQueueName){
  return new TMS_Data::ListQueues();
}


void
PosixServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                            const std::vector<std::string>& ignoredIds){
}


PosixServer::~PosixServer(){
}

int
create_plugin_instance(void **instance) {
  try {
    *instance = new PosixServer;
  } catch (const std::bad_alloc& e) {
    return 1;
  }

  return PLUGIN_OK;
}

