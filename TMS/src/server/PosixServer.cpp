#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include <iostream>

#include "PosixServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"

#include "tms-posix.hpp"

PosixServer::PosixServer():BatchServer(){
}


int
PosixServer::submit(const char* scriptPath,
                    const TMS_Data::SubmitOptions& options,
                    TMS_Data::Job& job,
                    char** envp){
  int ret;
  struct st_job resultat;

  buildEnvironment();
  LaunchDaemon();

std::cout<<"Submit"<<std::endl;
  ret = ReqSubmit(scriptPath, &resultat);
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
