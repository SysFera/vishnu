/**
 * \file LSFServer.hpp
 * \brief This file contains the VISHNU LSFServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2012
 */


#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include <sys/types.h>
#include <pwd.h>

#include "boost/filesystem.hpp"
#include<boost/algorithm/string.hpp>

#include <lsf/lsbatch.h>

#include "LSFServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"
#include "LSFParser.hpp"

using namespace std;
using namespace vishnu;
namespace bfs=boost::filesystem;
namespace ba=boost::algorithm;
/**
 * \brief Constructor
 */
LSFServer::LSFServer():BatchServer() {
  msymbolMap["\%J"] = "";
}

/**
 * \brief Function to submit LSF job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param job The job data structure
 * \param envp The list of environment variables used by LSF submission function 
 * \return raises an exception on error
 */
int 
LSFServer::submit(const char* scriptPath, 
    const TMS_Data::SubmitOptions& options, 
    TMS_Data::Job& job, char** envp) {

  struct submit  req;
  struct submitReply  reply;
  LS_LONG_INT  batchJobId;
  int  i;

  if (lsb_init(NULL) < 0) {
    lsb_perror((char*)"LSFServer::submit: lsb_init() failed");
    return -1;//error messages are written to stderr, VISHNU redirects these messages into a file
  }

  /* Initialize 0 in req */
  memset(&req, 0, sizeof(struct submit));

  req.options = 0;
  req.options2 = 0;
  req.options3 = 0;
  req.resReq = NULL;

  for (i = 0; i < LSF_RLIM_NLIMITS; i++)
    req.rLimits[i] = DEFAULT_RLIMIT;

  req.hostSpec = NULL;
  req.numProcessors = 1;
  req.maxNumProcessors = 1;
  req.beginTime = 0;
  req.termTime  = 0;
  std::string cmd = std::string(vishnu::get_file_content(scriptPath));
  std::cout << "********cmd=" << strdup(cmd.c_str()) << std::endl; 
  req.command = strdup(cmd.c_str());
  req.nxf = 0;
  req.delOptions = 0;
  req.numAskedHosts = 0;
  //parse the file
  if(LSFParser::parse_file(scriptPath, &req) < 0) {
    return -1; 
  };
  //processes the vishnu options
  processOptions(scriptPath, options, &req);

  if(req.jobName!=NULL) std::cout << "********req.jobName=" << req.jobName << std::endl; 
  if(req.outFile!=NULL) {
     bfs::path outPath(bfs::system_complete(bfs::path(req.outFile)));
     req.outFile = strdup((outPath.native()).c_str()) ;
     std::cout << "********req.outFile=" << req.outFile << std::endl;
  }
  if(req.errFile!=NULL) {
     bfs::path errPath(bfs::system_complete(bfs::path(req.errFile)));
     req.errFile = strdup((errPath.native()).c_str()) ;
     std::cout << "********req.errFile=" << req.errFile << std::endl;
  }
  //Check the job output path 
  std::string errorMsg = checkLSFOutPutPath(req.outFile);
  if(errorMsg.size()!=0) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
  }
  //Check the job error path
  errorMsg = checkLSFOutPutPath(req.errFile, "job error path");
  if(errorMsg.size()!=0) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
  }

  batchJobId = selectQueueAutom(options, &req, &reply);
  if(batchJobId < 0){  

    batchJobId = lsb_submit(&req, &reply);

    if (batchJobId < 0) {
      switch (lsberrno) {
        case LSBE_QUEUE_USE:
        case LSBE_QUEUE_CLOSED:
          lsb_perror(reply.queue);
          return -1;//error messages are written to stderr, VISHNU redirects these messages into a file
        default:
        lsb_perror(NULL);
        return -1;//error messages are written to stderr, VISHNU redirects these messages into a file
     }
    }
  }
  if(req.jobName!=NULL) std::cout << "********2req.jobName=" << req.jobName << std::endl;
  if(req.outFile!=NULL) std::cout << "********2req.outFile=" << req.outFile << std::endl;
  std::cout << "**********batchJobId= " << batchJobId << std::endl;
  int numJobs = lsb_openjobinfo(batchJobId, NULL, NULL, NULL, NULL, JOBID_ONLY);
  struct jobInfoEnt *jobInfo = lsb_readjobinfo(&numJobs);
  lsb_closejobinfo();
  if (jobInfo == NULL) {
    //error messages are written to stderr, VISHNU redirects these messages into a file
    lsb_perror((char*)"LSFServer::submit: lsb_redjobinfo() failed");
    return -1;
  }

  std::string jobOutputPath ;
  std::string jobErrorPath;
  if(req.outFile!=NULL) {
    jobOutputPath = req.outFile;
  }
  if(req.errFile!=NULL) {
    jobErrorPath = req.errFile;
  }

  //Fill the vishnu job structure
  fillJobInfo(job, jobInfo);

  if(!jobOutputPath.empty()) {
    replaceSymbolInToJobPath(jobOutputPath);
    job.setOutputPath(jobOutputPath);
  }
  if(!jobErrorPath.empty()) {
    replaceSymbolInToJobPath(jobErrorPath);
    job.setErrorPath(jobErrorPath);
  }

  if(req.jobName!=NULL) std::cout << "********jobOutputPath=" << jobOutputPath << ": " << req.jobName << std::endl;
  if(req.outFile!=NULL) std::cout << "********jobErrorPath=" << jobErrorPath << ": " << req.jobName << std::endl;

  return 0;
}

LS_LONG_INT  
LSFServer::selectQueueAutom(const TMS_Data::SubmitOptions& options, struct submit* req, struct submitReply *reply) {

  LS_LONG_INT batchJobId=-1;
  if(options.isSelectQueueAutom()) {

    TMS_Data::ListQueues* listOfQueues = listQueues();
    if(listOfQueues != NULL) {

      for(unsigned int i = 0; i < listOfQueues->getNbQueues(); i++) {
         TMS_Data::Queue* queue =  listOfQueues->getQueues().get(i);
         req->options |=SUB_QUEUE;
         req->queue = strdup(queue->getName().c_str()); 
         std::cout << "==============================Candidated queue name is :" << req->queue << std::endl;
         batchJobId = lsb_submit(&(*req), &(*reply));

         if (batchJobId < 0) {
            if(lsberrno==LSBE_QUEUE_USE || lsberrno==LSBE_QUEUE_WINDOW 
              || lsberrno==LSBE_QUEUE_HOST || lsberrno==LSBE_QUEUE_CLOSED)
             {
                if(i==listOfQueues->getNbQueues()){
                   throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "LSF: not queue corresponding"
                         " of the constraint to submit this job.");
                }
                continue;
             } else {
               lsb_perror(NULL);
               batchJobId=-1;
               break;
             }
          }
          break;
      }
    }
  }
  if(req->queue!=NULL) std::cout << "==============================Selected queue name is :" << req->queue << std::endl;
  return batchJobId;
}

/**
 * \brief Function to treat the submission options
 * \param scriptPath The job script path
 * \param options the object which contains the SubmitOptions options values
 * \param struct submit The LSF submit request
 * \return raises an exception on error
 */
void 
LSFServer::processOptions(const char* scriptPath,
    const TMS_Data::SubmitOptions& options, 
    struct submit* req) {

  if(!options.getNbNodesAndCpuPerNode().empty() && options.getNbCpu()!=-1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the NbCpu option and NbNodesAndCpuPerNode option together.\n");
  }

  if(options.isSelectQueueAutom() && !options.getQueue().empty() ) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the SelectQueueAutom (-Q) and getQueue (-q) options together.\n");
  }

  if(!options.getName().empty()){
    req->options |=SUB_JOB_NAME;
    req->jobName = strdup(options.getName().c_str());
  } 
  if(!options.getQueue().empty()) {
    req->options |=SUB_QUEUE;
    req->queue = strdup(options.getQueue().c_str());
  }
  if(!options.getOutputPath().empty()) {
    req->options |=SUB_OUT_FILE;
    req->outFile = strdup(options.getOutputPath().c_str());
  }
  if(!options.getErrorPath().empty()) {
    req->options |=SUB_ERR_FILE;
    req->errFile = strdup(options.getErrorPath().c_str());
  }
  if(options.getWallTime()!=-1) {
    req->rLimits[LSF_RLIMIT_RUN] = options.getWallTime()/60;
  }
  if(options.getNbCpu()!=-1) {
    req->numProcessors = options.getNbCpu();
    req->maxNumProcessors= options.getNbCpu();  
  }
  if(options.getMemory()!=-1) {
    req->options |=SUB_RES_REQ;
    req->rLimits[LSF_RLIMIT_RSS] = options.getMemory(); //To complete later
  }
  if(!(options.getNbNodesAndCpuPerNode().empty())) {
    std::string NbNodesAndCpuPerNode = options.getNbNodesAndCpuPerNode();
    size_t posNbNodes = NbNodesAndCpuPerNode.find(":");
    if(posNbNodes!=std::string::npos) {
      std::string nbNodesStr = NbNodesAndCpuPerNode.substr(0, posNbNodes);
      std::string cpuPerNode = NbNodesAndCpuPerNode.substr(posNbNodes+1); 

      //set the number of processor     
      req->numProcessors = vishnu::convertToInt(cpuPerNode);
      req->maxNumProcessors = req->numProcessors; 
    
      struct hostInfoEnt *hostInfo;
      char **hosts = NULL;
      int numhosts = 0;
      int nbNodes = vishnu::convertToInt(nbNodesStr);
      //select the candidates host
      hostInfo = lsb_hostinfo(hosts, &numhosts);
      std::cout << "-----------------------numhosts=" << numhosts << std::endl;
      if(nbNodes > numhosts) {
        throw UserException(ERRCODE_BATCH_SCHEDULER_ERROR, "LSF ERRROR: "
                  "The number of nodes is greater than the number of total nodes."); 
      }
      if(req->numAskedHosts > 0) {
        delete [] req->askedHosts;
      }
      req->options |=SUB_HOST;//Set LSF option
      req->askedHosts = new char*[nbNodes];
      req->numAskedHosts = nbNodes;
      for (int i = 0; i < nbNodes; i++, hostInfo++) {
          req->askedHosts[i] = hostInfo->host;
      } 
    } 
  }

  if(!(options.getMailNotification().empty())) {
    std::string notification = options.getMailNotification();
    if(notification.compare("BEGIN")==0) {
       req->options |=SUB_NOTIFY_BEGIN;
    } else if(notification.compare("END")==0) {
       req->options |=SUB_NOTIFY_END;
    } else if(notification.compare("ERROR")==0) {//not exist in LSF
       req->options |=SUB_NOTIFY_END; //send mail after execution or failure of the job
    } else if(notification.compare("ALL")==0) {
      req->options |=SUB_NOTIFY_BEGIN;
      req->options |=SUB_NOTIFY_END;
    } else {
      throw UserException(ERRCODE_INVALID_PARAM, notification+" is an invalid notification type:"+" consult the vishnu user manuel");
    }
  }

  if(!(options.getMailNotifyUser().empty())) {
    req->options |=SUB_MAIL_USER;
    req->mailUser = strdup(options.getMailNotifyUser().c_str());
  }

  if(!(options.getGroup().empty())) {
     req->options |=SUB_USER_GROUP;
     req->userGroup = strdup(options.getGroup().c_str());
  }

  if(!(options.getWorkingDir().empty())) {
     req->options3 |= SUB3_CWD;
     req->cwd = strdup(options.getWorkingDir().c_str());
   }

  if(!(options.getCpuTime().empty())) {
     req->rLimits[LSF_RLIMIT_CPU] = convertStringToWallTime(options.getCpuTime())/60;
  }

}

/**
 * \brief Function to check if LSF path syntax is correct
 * \param path The path to check
 * \param pathInfo The information on path to print
 * \return an error message
 */
std::string LSFServer::checkLSFOutPutPath(char*& path, const std::string& pathInfo) {

  string errorMsg;
  if(path!=NULL) {
    //get the LSF treated symbols
    std::map<std::string, std::string>::const_iterator iter;
    std::map<std::string, std::string>::const_iterator end=msymbolMap.end();
    std::string vishnuTreatedSymbols;

    std::string symbol;
    if(containsAnExcludedLSFSymbols(path, symbol)){
      ostringstream osStr;
      osStr << "VISHNU can't treats in your " << pathInfo << " the following sumbol: " << symbol << std::endl;

      for(iter=msymbolMap.begin(); iter!=end; ++iter) {
        if(iter!=msymbolMap.begin()) {
          vishnuTreatedSymbols+=" or ";
        }
        vishnuTreatedSymbols+=iter->first[0]+symbol.substr(1,symbol.size()-2)+iter->first[1];
      }
      osStr << "*****The only LSF symbols treated by VISHNU are: " << vishnuTreatedSymbols << std::endl;
      osStr << "*****Replace your symbols by the following sumbols: "<< vishnuTreatedSymbols << std::endl;
      errorMsg = osStr.str();
    }
  }
  return errorMsg;
}

/**
 * \brief Function to replace LSF job identifer symbol by its real value in to a path
 * \param path The path containing the job symbol
 */
void LSFServer::replaceSymbolInToJobPath(std::string& path) {

  std::string widthStr;
  std::ostringstream os;

  std::map<std::string, std::string>::const_iterator iter;
  std::map<std::string, std::string>::const_iterator end=msymbolMap.end();

  for(iter=msymbolMap.begin(); iter!=end; ++iter) {
    //find the symbol position 
    size_t pos0 = path.find((iter->first)[0]);
    size_t pos1 = path.find((iter->first)[1], pos0);
    while(pos0!=std::string::npos && pos1!=std::string::npos) {
      widthStr = path.substr(pos0+1, pos1-pos0-1);
      if(widthStr.size()==0) {
        path.erase(pos0, 2);//remove symbol[0]+symbol[1]
        path.insert(pos0, iter->second);
      } 
      //Pass to the next symbol
      pos0 = path.find(iter->first[0], pos0+1);
      pos1 = path.find(iter->first[1],pos0);
      os.str("");
    }
  }
}

/**
 * \brief Function to cheick if a path contains an excluded LSF symbol by vishnu
 * \param path The path to check
 * \param symbol The excluded symbol
 * \return true if the path contain an exluded symbol
 */
bool LSFServer::containsAnExcludedLSFSymbols(const std::string& path, std::string& symbol) {

  std::vector<std::string> excludedSymbols;
  excludedSymbols.push_back("\%I");

  std::vector<std::string>::const_iterator iter;
  std::vector<std::string>::const_iterator end=excludedSymbols.end();
  bool ret = false;
  std::string widthStr;
  for(iter=excludedSymbols.begin(); iter!=end; ++iter) {
    //find the symbol position 
    size_t pos0 = path.find((*iter)[0]);
    size_t pos1 = path.find((*iter)[1], pos0);
    while(pos0!=std::string::npos && pos1!=std::string::npos) {
      widthStr = path.substr(pos0+1, pos1-pos0-1);
      if(widthStr.size()==0) {
        ret = true;
        symbol = *iter;
        break;
      }
      //Pass to the next symbol
      pos0 = path.find((*iter)[0], pos0+1);
      pos1 = path.find((*iter)[1],pos0);
    }
  }
  return ret;
}


/**
 * \brief Function To convert vishnu job Id to LSF job Id 
 * \param jobId: vishnu job Id
 * \return the converted LSF job id
 */
LS_LONG_INT LSFServer::convertToLSFJobId(const std::string& jobId) {

  LS_LONG_INT lsfJobId;
  std::istringstream is(jobId);
  is >> lsfJobId;

  return lsfJobId;
}

/**
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int
LSFServer::cancel(const char* jobId) {

  LS_LONG_INT lsfJobId = convertToLSFJobId(jobId); 
  if (lsb_init(NULL) < 0) {
    lsb_perror((char*)"LSFServer::cancel: lsb_init() failed");
    return -1;//error messages are written to stderr, VISHNU redirects these messages into a file
  }

  int res = lsb_signaljob(lsfJobId, SIGKILL);
  if(res) {
    lsb_perror(NULL);
    return res;//error messages are written to stderr, VISHNU redirects these messages into a file
  }

  return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobId the identifier of the job 
 * \return -1 if the job is unknown or server not  unavailable 
 */
int 
LSFServer::getJobState(const std::string& jobId) {

  int state = 5; //TERMINATED 
  LS_LONG_INT lsfJobId = convertToLSFJobId(jobId);

  struct jobInfoEnt *jobInfo;
  int numJobs;

  if (lsb_init(NULL) < 0) {
    return state;
  }

  numJobs = lsb_openjobinfo(lsfJobId, NULL, NULL, NULL, NULL, JOBID_ONLY);
  jobInfo = lsb_readjobinfo(&numJobs);
  lsb_closejobinfo();

  if (jobInfo == NULL) {
    return state;
  }

  state = convertLSFStateToVishnuState(jobInfo->status);

  return state;
}

/**
 * \brief Function to get the start time of the job
 * \param jobId the identifier of the job 
 * \return 0 if the job is unknown
 */
time_t 
LSFServer::getJobStartTime(const std::string& jobId) {

  time_t startTime = 0; 

  LS_LONG_INT lsfJobId = convertToLSFJobId(jobId);
  struct jobInfoEnt *jobInfo;
  int numJobs;

  if (lsb_init(NULL) < 0) {
    return startTime;
  }

  numJobs = lsb_openjobinfo(lsfJobId, NULL, NULL, NULL, NULL, JOBID_ONLY);
  jobInfo = lsb_readjobinfo(&numJobs);
  lsb_closejobinfo();

  if (jobInfo == NULL) {
    return startTime;
  }

  startTime = jobInfo->startTime;
  return startTime;
} 

/**
 * \brief Function to convert the LSF state into VISHNU state 
 * \param state the state to convert
 * \return VISHNU state 
 */
int 
LSFServer::convertLSFStateToVishnuState(const unsigned int& state) {

  int res = 0;
  switch(state) {
    case JOB_STAT_PEND:case JOB_STAT_PSUSP:case JOB_STAT_SSUSP:case JOB_STAT_USUSP:case JOB_STAT_WAIT:
      res = 3;//WAITING
      break;
    case JOB_STAT_RUN:
      res = 4;//RUNNING
      break;
    case JOB_STAT_DONE:case JOB_STAT_PDONE:case JOB_STAT_PERR: case JOB_STAT_UNKWN:
      res = 5; //TERMINATED
      break;
    case JOB_STAT_EXIT:
      res = 6; //CANCELLED
      break;
    default:
      res = 5;
      break;
  }
  return res;
}

/**
 * \brief Function to convert the LSF priority into VISHNU priority
 * \param prio the priority to convert
 * \return VISHNU state 
 */
int 
LSFServer::convertLSFPrioToVishnuPrio(const uint32_t& prio) {
  if(prio < -512) {
    return 1;
  } else if(prio >= -512 && prio < 0) {
    return 2;
  } else if(prio >= 0 && prio < 512) {
    return 3;
  } else if(prio >= 512 && prio < 1023) {
    return 4;
  } else if(prio >= 1023) {
    return 5;
  }
  return 1; 
}

/**
 * \brief Function To fill the info concerning a job
 * \fn void fillJobInfo(TMS_Data::Job_ptr job, struct batch_status *p)
 * \param job: The job to fill
 * \param jobInfo: The LSF job structure information
 */
void
LSFServer::fillJobInfo(TMS_Data::Job &job, struct jobInfoEnt* jobInfo){

  job.setJobId(lsb_jobid2str(jobInfo->jobId));
  job.setBatchJobId(lsb_jobid2str(jobInfo->jobId));
  /*if(jobInfo->cwd!=NULL) {
    job.setOutputPath(std::string(jobInfo->cwd)+"/LSF-"+std::string(lsb_jobid2str(jobInfo->jobId))+".out");//default path
    job.setErrorPath(std::string(jobInfo->cwd)+"/LSF-"+std::string(lsb_jobid2str(jobInfo->jobId))+".err");//default path
    }*/
  job.setStatus(convertLSFStateToVishnuState(jobInfo->status));
  if(jobInfo->submit.jobName!=NULL){
    job.setJobName(jobInfo->submit.jobName);
  }
  job.setSubmitDate(jobInfo->submitTime);
  if(jobInfo->user!=NULL){
    job.setOwner(jobInfo->user);
  }
  if(jobInfo->submit.queue!=NULL){
    job.setJobQueue(jobInfo->submit.queue);
  }
  job.setWallClockLimit(jobInfo->submit.rLimits[LSF_RLIMIT_RUN]*60);
  job.setEndDate(jobInfo->endTime);
  if(jobInfo->submit.userGroup!=NULL){
    job.setGroupName(jobInfo->submit.userGroup);
  }
  if(jobInfo->submit.jobDescription!=NULL){
    job.setJobDescription(jobInfo->submit.jobDescription);
  }
  job.setJobPrio(convertLSFPrioToVishnuPrio(jobInfo->jobPriority));
  //mem limit
  job.setMemLimit(jobInfo->submit.rLimits[LSF_RLIMIT_RSS]);
  
  int nbCpu = jobInfo->submit.numProcessors;
  job.setNbCpus(nbCpu);
  //compte the number of unique nodes
  std::vector<std::string> tmpHosts;
  for(int i=0; i < jobInfo->submit.numAskedHosts; i++) {
    if(jobInfo->submit.askedHosts[i]!=NULL) {
      tmpHosts.push_back(jobInfo->submit.askedHosts[i]);
    }
  }
  std::vector<std::string>::iterator endTmp=std::unique(tmpHosts.begin(), tmpHosts.end());
  int node = endTmp-tmpHosts.begin();
 
  job.setNbNodes(node);
  if(node!=-1) {
    job.setNbNodesAndCpuPerNode(vishnu::convertToString(node)+":"+vishnu::convertToString(nbCpu));
  }
  //To fill the job working dir
  if(jobInfo->cwd!=NULL){
    job.setJobWorkingDir(jobInfo->cwd);
  }

  //fill the msymbol map
  msymbolMap["\%J"] = lsb_jobid2str(jobInfo->jobId); 
}

/**
 * \brief Function to request the status of queues 
 * \param optQueueName (optional) the name of the queue to request 
 * \return The requested status in to ListQueues data structure 
 */
TMS_Data::ListQueues*
LSFServer::listQueues(const std::string& OptqueueName) { 

  struct queueInfoEnt *queueInfo;
  char **queues = NULL;
  int numQueues = 0;
  char *host = NULL;
  char *user = NULL;
  int options = 0;

  if (lsb_init(NULL) < 0) {
    //error messages are written to stderr, VISHNU redirects these messages into a file
    lsb_perror((char*)"LSFServer::listQueues: lsb_init() failed");
    return mlistQueues;
  }
  if(!OptqueueName.empty()) {
   queues = new char*[1];
   queues[0]= strdup(OptqueueName.c_str());
   numQueues = 1; 
   queueInfo = lsb_queueinfo(queues, &numQueues, host, user, options);
   delete queues;
  } else {
    queueInfo = lsb_queueinfo(queues, &numQueues, host, user, options);
  }

  if (queueInfo == NULL) {
    //error messages are written to stderr, VISHNU redirects these messages into a file
    lsb_perror((char*)"listQueues: lsb_queueinfo() failed");
    return mlistQueues;
  }

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistQueues = ecoreFactory->createListQueues();

  for (int i = 0; i < numQueues; i++, queueInfo++) {

    TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();

    if (queueInfo->qStatus & QUEUE_STAT_ACTIVE) {
      if (!queueInfo->qStatus & QUEUE_STAT_OPEN) {
        queue->setState(0);
      } 
      if (queueInfo->qStatus & QUEUE_STAT_RUN) {
        queue->setState(2);
      } else {
        queue->setState(1);
      }
    } else {
      queue->setState(1);
    }

    queue->setName(queueInfo->queue);
    queue->setPriority(convertLSFPrioToVishnuPrio(queueInfo->priority));
    queue->setNbRunningJobs(queueInfo->numRUN);
    queue->setNbJobsInQueue(queueInfo->numJobs-queueInfo->numRUN);
    queue->setDescription(queueInfo->description);
    queue->setMemory(queueInfo->rLimits[LSF_RLIMIT_RSS]);
    queue->setMaxProcCpu(queueInfo->rLimits[LSF_RLIMIT_CPU]);
    queue->setMaxJobCpu(-1);//Undefined
    queue->setNode(queueInfo->procLimit);

    // Adding created queue to the list
    mlistQueues->getQueues().push_back(queue);
  }

  mlistQueues->setNbQueues(mlistQueues->getQueues().size());

  return mlistQueues;

}

/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore 
 */
void LSFServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
    const std::vector<string>& ignoredIds) {

  if (lsb_init(NULL) < 0) {
    lsb_perror((char*)"LSFServer::fillListOfJobs: lsb_init() failed");
    return;
  }

  int numJobs = lsb_openjobinfo(0, NULL, (char*)"all", NULL, NULL, CUR_JOB);
  std::cout << "*****************numJobs=" << numJobs << std::endl;
  if(numJobs < 0 ){
    if(lsberrno==LSBE_NO_JOB) {
      return;
    } else {
      lsb_perror((char*)"LSFServer::fillListOfJobs: lsb_openjobinfo failed");
    }
  }

  int more = 1; 
  struct jobInfoEnt *jobInfo;
  int jobStatus;
  long nbRunningJobs = 0;
  long nbWaitingJobs = 0;
  while(more) {
    jobInfo = lsb_readjobinfo(&more);
    if (jobInfo == NULL) {
      lsb_perror((char*)"LSFServer::fillListOfJobs: lsb_readjobinfo failed");
      return;
    } 
    std::vector<std::string>::const_iterator iter;
    iter = std::find(ignoredIds.begin(), ignoredIds.end(), convertToString(jobInfo->jobId));
    if(iter==ignoredIds.end()) {
      TMS_Data::Job_ptr job = new TMS_Data::Job();
      fillJobInfo(*job, jobInfo);
      jobStatus = job->getStatus();
      if(jobStatus==4) {
        nbRunningJobs++;
      } else if(jobStatus >= 1 && jobStatus <= 3) {
        nbWaitingJobs++;
      }
      listOfJobs->getJobs().push_back(job);
    }
  }
  lsb_closejobinfo();
  listOfJobs->setNbJobs(listOfJobs->getJobs().size());
  listOfJobs->setNbRunningJobs(listOfJobs->getNbRunningJobs()+nbRunningJobs);
  listOfJobs->setNbWaitingJobs(listOfJobs->getNbWaitingJobs()+nbWaitingJobs);

}

/**
 * \brief Destructor
 */
LSFServer::~LSFServer() {
}

