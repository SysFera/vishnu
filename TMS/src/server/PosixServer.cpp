#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include <iostream>
#include <unistd.h>
#include <string.h>

#include <boost/lexical_cast.hpp>

#include "PosixServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"
#include "tmsUtils.hpp" // For convertStringToWallTime
#include "constants.hpp"
#include "TmsPosixClient.hpp"

PosixServer::PosixServer():BatchServer(){
}


int
PosixServer::submit(const std::string& scriptPath,
                    const TMS_Data::SubmitOptions& options,
                    TMS_Data::ListJobs& jobSteps,
                    char** envp){
  int ret;
  struct trameJob resultat;
  struct trameSubmit op;
  std::string errorMsg;
  std::string strRet;

  memset(&op, 0, sizeof(op));
  strncpy(op.name, options.getName().c_str(), sizeof(op.name)-1);
  op.walltime = options.getWallTime();
  strncpy(op.outPutPath, options.getOutputPath().c_str(), sizeof(op.outPutPath)-1);
  strncpy(op.errorPath, options.getErrorPath().c_str(), sizeof(op.errorPath)-1);
  strncpy(op.workDir, options.getWorkingDir().c_str(), sizeof(op.workDir)-1);

  switch(fork()) {
  case -1:
    return -1;
  case 0:
    launchDaemon();
    exit(0);
    break;
  default:
    sleep(3); // TODO : fix, sleep because need synchronisation and can't wait child that has become a daemon
    break;
  }

  TMS_Data::Job_ptr jobPtr = new TMS_Data::Job();
  jobPtr->setStatus(vishnu::STATE_RUNNING);
  jobPtr->setJobQueue("posix");

  // If no name give a default job name
  if (options.getName().empty()){
    jobPtr->setJobName("posix_job");
  } else {
    jobPtr->setJobName(options.getName());
  }

  strncpy(op.jobName, jobPtr->getJobName().c_str(), sizeof(op.jobName)-1);

  ret = reqSubmit(scriptPath, &resultat, &op);

  if (ret == 0) {
    jobPtr->setOutputPath(std::string(resultat.outPutPath));
    jobPtr->setBatchJobId(std::string(resultat.jobId));

    jobPtr->setErrorPath(std::string(resultat.errorPath));
    jobPtr->setWallClockLimit(resultat.maxTime);

    jobSteps.getJobs().push_back(jobPtr);
  }
  else {
    strRet = boost::lexical_cast<std::string>(ret);
    errorMsg = "Error submiting job  error : "  + strRet + " ernno :" + std::string(strerror(errno));
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "POSIX ERROR: "+errorMsg);
  }
  return ret;
}


int
PosixServer::cancel(const std::string& jobId){
  return reqCancel(jobId);
}


int
PosixServer::getJobState(const std::string& jobId){
  struct trameJob resultat;
  int ret;

  ret = reqInfo(jobId.c_str(), &resultat);

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
  struct trameJob resultat;
  reqInfo(jobId.c_str(), &resultat);
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
