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

  memset(&op,0,sizeof(op));
  memcpy(op.name, options.getName().c_str(), strlen(options.getName().c_str()));
  op.walltime = options.getWallTime();
  memcpy(op.OutPutPath, options.getOutputPath().c_str(), strlen(options.getOutputPath().c_str()));
  memcpy(op.ErrorPath, options.getErrorPath().c_str(), strlen(options.getErrorPath().c_str()));
  memcpy(op.WorkDir, options.getWorkingDir().c_str(), strlen(options.getWorkingDir().c_str()));

  std::cout << "OutPutPath: " << op.OutPutPath << std::endl;
  std::cout << "ErrorPath: " << op.ErrorPath << std::endl;
  std::cout << "WorkingDir: " << op.WorkDir << std::endl;

  buildEnvironment();

  std::cout << "PosixServerSubmit"<<std::endl;
  switch(fork()) {
    case -1:
      return -1;
    case 0:
  LaunchDaemon();
      break;
    default:
      sleep(3); // TODO : fix, sleep because need synchronisation and can't wait child that has become a daemon
      break;
  }

  std::cout << "PosixServer reqSubmit"<<std::endl;
  ret = ReqSubmit(scriptPath, &resultat, &op);

  std::cout << "PosixServer fin reqSubmit"<<std::endl;

  job.setBatchJobId(std::string(resultat.JobId));
  job.setJobId(std::string(resultat.JobId));
  job.setStatus(4);
  job.setJobQueue("posix");
  std::cout << "Job Status :" << job.getStatus() << std::endl;
// If no name give a default job name
  
  if (options.getName().compare("")==0){
    job.setJobName("posix_job");
  } else {
    job.setJobName(options.getName());
  }

  job.setOutputPath(op.OutPutPath);

/***
  if (options.getOutputPath().compare("")==0){
    job.setOutputPath("/tmp/out.o");
  } else {
    job.setOutputPath(options.getOutputPath());
  }
**/

  job.setErrorPath(op.ErrorPath);
/**
  if (options.getErrorPath().compare("")==0){
    job.setErrorPath("/tmp/err.e");
  } else {
    job.setErrorPath(options.getErrorPath());
  }
**/

 std::cout << "ret code : " << ret << std::endl;
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

