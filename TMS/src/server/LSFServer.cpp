/**
 * \file LSFServer.hpp
 * \brief This file contains the VISHNU LSFServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include <sys/types.h>
#include <pwd.h>

#include <boost/algorithm/string.hpp>

#include <lsf/lsbatch.h>

#include "LSFServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"

using namespace std;
using namespace vishnu;

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
    lsb_perror((char*)"vishnu_submit_job: lsb_init() failed");
    return -1;//error messages are written to stderr, VISHNU redirects these messages into a file
  }

  /* Initialize 0 in req */
  memset(&req, 0, sizeof(struct submit));

  req.options = 0;
  req.resReq = NULL;
  
  for (i = 0; i < LSF_RLIM_NLIMITS; i++)
    req.rLimits[i] = DEFAULT_RLIMIT;

  req.hostSpec = NULL;
  req.numProcessors = 1;
  req.maxNumProcessors = 1;
  req.beginTime = 0;
  req.termTime  = 0;
  std::string cmd = std::string(vishnu::get_file_content(scriptPath));
  std::cout << "********cmd=" << const_cast<char*>(cmd.c_str()) << std::endl; 
  req.command = const_cast<char*>(cmd.c_str());
  req.nxf = 0;
  req.delOptions = 0;
   
  //processes the vishnu options
  processOptions(scriptPath, options, &req);
 
  if(req.jobName!=NULL) std::cout << "********req.jobName=" << req.jobName << std::endl; 
  if(req.outFile!=NULL) std::cout << "********req.outFile=" << req.outFile << std::endl;
 
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

  if(jobOutputPath.size()!=0) {
    replaceSymbolInToJobPath(jobOutputPath);
    job.setOutputPath(jobOutputPath);
  }
  if(jobErrorPath.size()!=0) {
    replaceSymbolInToJobPath(jobErrorPath);
    job.setErrorPath(jobErrorPath);
  }

  
  return 0;
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

	if(options.getName().size()!=0){
		req->options |=SUB_JOB_NAME;
		req->jobName = const_cast<char*>(options.getName().c_str());
	} else {
		req->options |=SUB_JOB_NAME;
		req->jobName = const_cast<char*>(scriptPath);
	}
	if(options.getQueue().size()!=0) {
		req->options |=SUB_QUEUE;
		req->queue = const_cast<char*>(options.getQueue().c_str());
	}
	if(options.getOutputPath().size()!=0) {
		req->options |=SUB_OUT_FILE;
		req->outFile = const_cast<char*>(options.getOutputPath().c_str());
	}
	if(options.getErrorPath().size()!=0) {
		req->options |=SUB_ERR_FILE;
		req->errFile = const_cast<char*>(options.getErrorPath().c_str());
	}
	if(options.getWallTime()!=-1) {
		//cmdsOptions.push_back("-t"); 
		std::string timeStr = vishnu::convertWallTimeToString(options.getWallTime());
		size_t pos = timeStr.rfind(":");
		int i=0;
		while(pos!=std::string::npos){
			i++;
			if(i==3) {
				timeStr = timeStr.replace(pos, 1, "-");
				break;
			}
			if(pos==0) {
				break;
			} else {
				pos = timeStr.rfind(":", pos-1);
			}
		}

		//cmdsOptions.push_back(timeStr);
	}
	if(options.getNbCpu()!=-1) {
	}
	if(options.getMemory()!=-1) {
	}
	if(options.getNbNodesAndCpuPerNode()!="") {
		std::string NbNodesAndCpuPerNode = options.getNbNodesAndCpuPerNode();
		size_t posNbNodes = NbNodesAndCpuPerNode.find(":");
		if(posNbNodes!=std::string::npos) {
			std::string nbNodes = NbNodesAndCpuPerNode.substr(0, posNbNodes);
			std::string cpuPerNode = NbNodesAndCpuPerNode.substr(posNbNodes+1); 
			//cmdsOptions.push_back("--nodes="+nbNodes);
			//cmdsOptions.push_back("--mincpus="+cpuPerNode);
		}
	}

	if(options.getMailNotification()!="") {
		std::string notification = options.getMailNotification();
		if(notification.compare("BEGIN")==0) {
			//cmdsOptions.push_back("--mail-type=BEGIN");
		} else if(notification.compare("END")==0) {
			//cmdsOptions.push_back("--mail-type=END");
		} else if(notification.compare("ERROR")==0) {
			//cmdsOptions.push_back("--mail-type=FAIL");
		} else if(notification.compare("ALL")==0) {
			//cmdsOptions.push_back("--mail-type=ALL");
		} else {
			throw UserException(ERRCODE_INVALID_PARAM, notification+" is an invalid notification type:"+" consult the vishnu user manuel");
		}
	}

	if(options.getMailNotifyUser()!="") {
		//cmdsOptions.push_back("--mail-user="+options.getMailNotifyUser());
	}

	if(options.getGroup()!="") {
		//cmdsOptions.push_back("--gid="+options.getGroup());
	}

	if(options.getWorkingDir()!="") {
		//cmdsOptions.push_back("-D");
		//cmdsOptions.push_back(options.getWorkingDir());
	}

	if(options.getCpuTime()!="") {
		//cmdsOptions.push_back("-t");
		//cmdsOptions.push_back(options.getCpuTime());
	}

	if(options.isSelectQueueAutom()) {
		int node = 0;
		int cpu = -1;
		istringstream isNode;
		std::string optionNodesValue = options.getNbNodesAndCpuPerNode();
		if(optionNodesValue.empty()) {
			std::string nodeStr = ""/*getLSFResourceValue(scriptPath, "-N", "--nodes")*/;
			std::string cpuStr =  ""/*getLSFResourceValue(scriptPath, "", "--mincpus")*/;
			if(!nodeStr.empty()) {
				if(nodeStr.find('-')!=std::string::npos) {
					istringstream isNodeStr(nodeStr);
					int minnode;
					int maxNode;
					char sparator;
					isNodeStr >> minnode;
					isNodeStr >> sparator;
					isNodeStr >> maxNode;
					node = maxNode; 
				} else {
					node = vishnu::convertToInt(nodeStr);
				}
			}
			if(!cpuStr.empty()) {
				cpu = vishnu::convertToInt(cpuStr);
			}
			if(options.getNbCpu()!=-1) {
				cpu=options.getNbCpu();
			} 
		} else {
			isNode.str(optionNodesValue);
			isNode >> node;
			char colon;
			isNode >> colon;
			isNode >> cpu;
		}
		if(node <=0) {
			node = 1;
		}
		TMS_Data::ListQueues* listOfQueues = listQueues();
		if(listOfQueues != NULL) {
			for(unsigned int i = 0; i < listOfQueues->getNbQueues(); i++) {
				TMS_Data::Queue* queue =  listOfQueues->getQueues().get(i);
				if(queue->getNode()>=node){
					std::string queueName = queue->getName();

					std::string walltimeStr = ""/*getLSFResourceValue(scriptPath, "-t", "--time")*/;
					long walltime = options.getWallTime()==-1?vishnu::convertStringToWallTime(walltimeStr):options.getWallTime();
					long qwalltimeMax = queue->getWallTime();
					int qCpuMax = queue->getMaxProcCpu();

					if((walltime <= qwalltimeMax || qwalltimeMax==0) &&
							(cpu <= qCpuMax)){
						//cmdsOptions.push_back("-p");
						//cmdsOptions.push_back(queueName);
						break;
					}
				};
			}
		}
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
	int width;
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
        std::cout << "*****************lsfJobId=" << lsfJobId << std::endl;
	/*//int res = lsb_signaljob(lsfJobId, SIGKILL);
        //int res = lsb_forcekilljob(lsfJobId);
	std::cout << "*****************res=" << res << std::endl;
	if(res) {
		lsb_perror(NULL);
		return res;//error messages are written to stderr, VISHNU redirects these messages into a file
	}*/
        
	std::ostringstream cmd;
	std::string  cancelCommand="bkill";

	cmd << cancelCommand << " " << jobId;
	if(system((cmd.str()).c_str())) {
		return -1; //error messages are written to stderr, VISHNU redirects these messages into a file
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
	job.setWallClockLimit(jobInfo->submit.runtimeEstimation);
	job.setEndDate(jobInfo->endTime);
	if(jobInfo->submit.userGroup!=NULL){
		job.setGroupName(jobInfo->submit.userGroup);
	}
	if(jobInfo->submit.jobDescription!=NULL){
		job.setJobDescription(jobInfo->submit.jobDescription);
	}
	job.setJobPrio(convertLSFPrioToVishnuPrio(jobInfo->jobPriority));

	job.setMemLimit(jobInfo->submit.rLimits[LSF_RLIMIT_RSS]);
	int nbCpu = jobInfo->submit.numProcessors;
	job.setNbCpus(nbCpu);
	int node = jobInfo->submit.numAskedHosts;
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
      lsb_perror((char*)"listQueues: lsb_init() failed");
      return mlistQueues;
    }

    queueInfo = lsb_queueinfo(queues, &numQueues, host, user, options);

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
  int cpt = 0; 
  while(more) {
          std::cout << "************************************iter" << ++cpt << "*********" << std::endl; 
	  jobInfo = lsb_readjobinfo(&more);
          std::cout << "************************************after lsb_readjobinfo" << std::endl;
	  if (jobInfo == NULL) {
		  lsb_perror((char*)"LSFServer::fillListOfJobs: lsb_readjobinfo failed");
		  return;
	  } 
	  std::cout << "***********************more=" << more << std::endl;
	  std::vector<std::string>::const_iterator iter;
	  std::cout << "***********************jobInfo->jobId=" << jobInfo->jobId << std::endl;
	  iter = std::find(ignoredIds.begin(), ignoredIds.end(), convertToString(jobInfo->jobId));
	  std::cout << "***********************jobInfo->jobId=" << jobInfo->jobId << std::endl;
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
	  std::cout << "***********************jobInfo->jobId=" << jobInfo->jobId << std::endl;
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

