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
  std::string OutPutPath;
  std::string ErrorPath;

  memset(&op,0,sizeof(op));
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
      exit(0);
      break;
    default:
      sleep(3); // TODO : fix, sleep because need synchronisation and can't wait child that has become a daemon
      break;
  }

  ret = ReqSubmit(scriptPath, &resultat, &op);

  job.setBatchJobId(std::string(resultat.JobId));
  job.setJobId(std::string(resultat.JobId));
  job.setStatus(4);
  job.setJobQueue("posix");
// If no name give a default job name
  
  if (options.getName().compare("")==0){
    job.setJobName("posix_job");
  } else {
    job.setJobName(options.getName());
  }

  OutPutPath = std::string(resultat.OutPutPath);
  ErrorPath = std::string(resultat.ErrorPath);

  job.setOutputPath(OutPutPath);

  job.setErrorPath(ErrorPath);

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
  switch (resultat.state){
  case DEAD:
  case TERMINATED:
  case ZOMBIE:
  case KILL:
  case KILL9:
    ret = 5;
    break;
  case RUNNING:
    ret = 4;
    break;
  case WAITING:
    ret = 3;
    break;
  default:
    ret = 5;
    break;
  }

  return ret;
}


time_t
PosixServer::getJobStartTime(const std::string& jobId){
  struct st_job resultat;
  int ret;

  ret = ReqInfo(jobId.c_str(), &resultat);
  return resultat.startTime;
}


TMS_Data::ListQueues*
PosixServer::listQueues(const std::string& optQueueName){
  TMS_Data::ListQueues* res = new TMS_Data::ListQueues();
  TMS_Data::Queue * q = new TMS_Data::Queue();
  q->setName("posix");
  q->setMaxJobCpu(1);
  q->setMaxProcCpu(1);
  q->setMemory(0);
  q->setWallTime(0);
  q->setNode(1);
  q->setNbRunningJobs(0);
  q->setNbJobsInQueue(0);
  q->setState(2);
  q->setPriority(1);
  q->setDescription("Basic no queue");
  res->getQueues().push_back(q);
  return res;
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

