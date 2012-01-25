#include <string>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include "llapi.h"

#include "LLServer.hpp"

#include "TMSVishnuException.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;
using namespace std;

/**
 * \brief Constructor
 */
LLServer::LLServer():BatchServer() {
 mlistQueues = NULL;
}

/**
 * \brief Function to submit LL job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param job The job data structure
 * \param envp The list of environment variables used by LL submission function 
 * \return raises an exception on error
 */
int 
LLServer::submit(const char* scriptPath, 
                 const TMS_Data::SubmitOptions& options, 
                 TMS_Data::Job& job, char** envp) {

  //Puts the options values into the scriptPath
  processOptions(scriptPath, options);

  LL_job llJobInfo;
  if(llsubmit(const_cast<char*>(scriptPath), NULL, NULL, &llJobInfo, LL_JOB_VERSION)) {
    return -1 ;//error messages are written to stderr, VISHNU redirects these messages into a file
  };

  std::ostringstream llJobId;
  llJobId<< (llJobInfo.step_list[0])->id.from_host;
  llJobId<< "." << (llJobInfo.step_list[0])->id.cluster;
  llJobId<< "." << (llJobInfo.step_list[0])->id.proc;

  job.setJobId(llJobId.str());
  job.setOutputPath(std::string(std::string(llJobInfo.step_list[0]->iwd)+"/"+(llJobInfo.step_list[0])->out)) ;
  job.setErrorPath(std::string(std::string(llJobInfo.step_list[0]->iwd)+"/"+(llJobInfo.step_list[0])->err));
  job.setStatus(convertLLStateToVishnuState(llJobInfo.step_list[0]->status));
  job.setJobName(std::string(llJobInfo.job_name));
  job.setSubmitDate((llJobInfo.step_list[0])->q_date);
  job.setOwner(std::string(llJobInfo.owner));
  job.setJobQueue(std::string((llJobInfo.step_list[0])->stepclass));
  job.setWallClockLimit(llJobInfo.step_list[0]->limits.soft_wall_clock_limit);
  job.setEndDate(-1);
  job.setGroupName(std::string((llJobInfo.step_list[0])->group_name));
  job.setJobDescription(std::string((llJobInfo.step_list[0])->comment));
  job.setJobPrio(convertLLPrioToVishnuPrio((llJobInfo.step_list[0])->prio));

  job.setMemLimit(llJobInfo.step_list[0]->memory_requested);
  int nbCpu = llJobInfo.step_list[0]->cpus_per_core;
  job.setNbCpus(nbCpu);
  int node = llJobInfo.step_list[0]->max_processors;
  job.setNbNodes(node);
  if(node!=-1) { 
    job.setNbNodesAndCpuPerNode(vishnu::convertToString(node)+":"+vishnu::convertToString(nbCpu));
  }
  //To fill the job working dir
  job.setJobWorkingDir(llJobInfo.step_list[0]->iwd); 
  
  llfree_job_info(&llJobInfo,LL_JOB_VERSION);

  return 0;
}

/**
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int 
LLServer::cancel(const char* jobId) { 

  std::ostringstream cmd;
  std::string  cancelCommand="llcancel"; 
  
  cmd << cancelCommand << " " << jobId;
  if(system((cmd.str()).c_str())) { 
    return -1; //error messages are written to stderr, VISHNU redirects these messages into a file
  }
 
 return 0; 
}

/**
 * \brief Function to treat the submission options
 * \param scriptPath The job script path
 * \param options the object which contains the SubmitOptions options values
 * \return raises an exception on error
 */
void
LLServer::processOptions(const char* scriptPath, 
                         const TMS_Data::SubmitOptions& options) {

  if(!options.getNbNodesAndCpuPerNode().empty() && options.getNbCpu()!=-1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the NbCpu option and NbNodesAndCpuPerNode option together.\n");
  }

  if(options.isSelectQueueAutom() && !options.getQueue().empty() ) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the SelectQueueAutom (-Q) and getQueue (-q) options together.\n");
  }
 
  std::string content = vishnu::get_file_content(scriptPath); 
  std::string optionLineToInsert;
  if(content.size()!=0) {
    if(options.getName().size()!=0){
      optionLineToInsert ="# @ job_name="+options.getName()+"\n";
      insertOptionLine(optionLineToInsert, content); 
    }
    if(options.getQueue().size()!=0) {
      optionLineToInsert ="# @ class="+options.getQueue()+"\n";
      insertOptionLine(optionLineToInsert, content);
    }
    if(options.getOutputPath().size()!=0) {
      optionLineToInsert ="# @ output="+options.getOutputPath()+"\n";
      insertOptionLine(optionLineToInsert, content); 
    }
    if(options.getErrorPath().size()!=0) {
      optionLineToInsert ="# @ error="+options.getErrorPath()+"\n";
      insertOptionLine(optionLineToInsert, content);
    }
    if(options.getWallTime()!=-1) {
      optionLineToInsert ="# @ wall_clock_limit="+vishnu::convertWallTimeToString(options.getWallTime())+"\n";
      insertOptionLine(optionLineToInsert, content);
    }
    if(options.getNbCpu()!=-1) {
      optionLineToInsert ="# @ tasks_per_node = 1 \n";
      optionLineToInsert +="# @ tasks_affinity = core(1) \n";
      optionLineToInsert +="# @ cpus_per_node = "+vishnu::convertToString(options.getNbCpu())+"\n";
      insertOptionLine(optionLineToInsert, content);
    }
    if(options.getMemory()!=-1) {
      optionLineToInsert ="# @ resources = ConsumableMemory("+vishnu::convertToString(options.getNbCpu())+" mb)\n";
      insertOptionLine(optionLineToInsert, content);
    }
    if(options.getNbNodesAndCpuPerNode()!="") {
      std::string NbNodesAndCpuPerNode = options.getNbNodesAndCpuPerNode();
      size_t posNbNodes = NbNodesAndCpuPerNode.find(":");
      if(posNbNodes!=std::string::npos) {
        std::string nbNodes = NbNodesAndCpuPerNode.substr(0, posNbNodes);
        std::string cpuPerNode = NbNodesAndCpuPerNode.substr(posNbNodes+1);
        optionLineToInsert = "# @ node="+nbNodes+"\n";
        insertOptionLine(optionLineToInsert, content);  
        optionLineToInsert = "# @ tasks_per_node=1\n";
        insertOptionLine(optionLineToInsert, content);
        optionLineToInsert ="# @ tasks_affinity = core(1) \n";
        optionLineToInsert +="# @ cpus_per_node = "+cpuPerNode+")\n";
        insertOptionLine(optionLineToInsert, content);
      }
    }
    if(options.getMailNotification()!="") {
      std::string notification = options.getMailNotification();
      if(notification.compare("BEGIN")==0) {
        optionLineToInsert = "# @ notification=start\n"; 
        insertOptionLine(optionLineToInsert, content);
      } else if(notification.compare("END")==0) {
        optionLineToInsert = "# @ notification=complete\n"; 
        insertOptionLine(optionLineToInsert, content);
      } else if(notification.compare("ERROR")==0) {
        optionLineToInsert = "# @ notification=error\n";
        insertOptionLine(optionLineToInsert, content);
      } else if(notification.compare("ALL")==0) {
        optionLineToInsert = "# @ notification=always\n";
        insertOptionLine(optionLineToInsert, content);
      } else {
        throw UserException(ERRCODE_INVALID_PARAM, notification+" is an invalid notification type:"+" consult the vishnu user manuel");
      }
    }

    if(options.getMailNotifyUser()!="") {
      optionLineToInsert = "# @ notify_user="+options.getMailNotifyUser()+"\n";
      insertOptionLine(optionLineToInsert, content);
    }

    if(options.getGroup()!="") {
      optionLineToInsert = "# @ group="+options.getGroup()+"\n";
      insertOptionLine(optionLineToInsert, content);
    }

    if(options.getWorkingDir()!="") {
      optionLineToInsert = "# @ initialdir="+options.getWorkingDir()+"\n";
      insertOptionLine(optionLineToInsert, content);
    }

    if(options.getCpuTime()!="") {
      optionLineToInsert = "# @ cpu_limit="+options.getCpuTime()+"\n";
      insertOptionLine(optionLineToInsert, content);
    }

    if(options.isSelectQueueAutom()) {
      int node = 0;
      istringstream isNode;
      std::string optionNodesValue = options.getNbNodesAndCpuPerNode();
      if(optionNodesValue.empty()) {
        node = vishnu::convertToInt(getLLResourceValue(scriptPath, "node"));
      } else {
        isNode.str(optionNodesValue);
        isNode >> node;
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

            std::string walltimeStr = getLLResourceValue(scriptPath, "wall_clock_limit");
            long walltime = options.getWallTime()==-1?vishnu::convertStringToWallTime(walltimeStr):options.getWallTime();
            long qwalltimeMax = queue->getWallTime();

            if((walltime <= qwalltimeMax || qwalltimeMax==0)){
              optionLineToInsert ="# @ class="+queueName+"\n";
              insertOptionLine(optionLineToInsert, content);
              break;
            }
          };
        }
      }
    }

    if(optionLineToInsert.size()!=0) {
      ofstream ofs(scriptPath);
      ofs << content;
      ofs.close();
    }
  }
}

/**
 * \brief Function to insert option into string 
 * \param optionLineToInsert the option to insert 
 * \param content The buffer containing the inserted option
 * \return raises an exception on error
 */
void
LLServer::insertOptionLine(const std::string& optionLineToInsert, 
                           std::string& content) {

  size_t pos = 0;
  size_t posLastDirective = 0;

  while(pos!=string::npos) {

    pos = content.find("#", pos);
    if(pos!=string::npos) {

      std::string line = content.substr(pos, content.find("\n", pos)-pos);
      pos++;
      size_t pos1 = line.find("#");
      size_t pos2 = line.find("@");
      if((pos1!=string::npos) && (pos2!=string::npos)) {
        std::string space = line.substr(pos1+1, pos2-pos1-1);
        size_t spaceSize = space.size();
        int i = 0;
        while((i < spaceSize) && (space[i]==' ')) {
          i++;
        };

        if(i==spaceSize) {
          std::string line_tolower(line);
          std::transform(line.begin(), line.end(), line_tolower.begin(), ::tolower);
          if(line_tolower.find("queue")!=string::npos) {
            content.insert(posLastDirective, optionLineToInsert);
            pos = pos + optionLineToInsert.size()+1;
          }
        }
      }

      posLastDirective = pos+line.size();
    }
  }
}

/**
 * \brief Function to get the status of the job
 * \param jobId the identifier of the job 
 * \return -1 if the job is unknown or server not  unavailable 
 */
int
LLServer::getJobState(const std::string& jobId) {
 
  LL_element *queryObject;
  LL_element *queryInfos;
  LL_element *step;
  LL_element *machineObj;
  char *machine;
  int rc;
  int objCount;
  int state;
  int errCode;
  int holdType;

  // Set the type of query
  queryObject = ll_query(JOBS);
  if(!queryObject) {
    return 5;
  }

  char* IDlist[2];
  IDlist[0] = strdup(jobId.c_str());
  IDlist[1] = NULL;

  // Create the request
  rc = ll_set_request(queryObject, QUERY_STEPID, IDlist, ALL_DATA);

  if(rc) {
    return 5;
  }
  if(queryObject==NULL) {
    return 5;
  }

  // Calling to get the results
  queryInfos = ll_get_objs(queryObject, LL_CM, NULL, &objCount, &errCode);
  if(queryInfos==NULL) {
    return 5;
  }

  while(queryInfos)
  {
    rc = ll_get_data(queryInfos, LL_JobGetFirstStep, &step);
    if(!rc)
    {
      while(step) {
        rc = ll_get_data(step, LL_StepState, &state);
        if(!rc) {
          int res = 0;
          switch(state) {
            case STATE_NOTQUEUED:
              res = 1;
              break; 
            case STATE_PENDING:case STATE_HOLD:
              res = 2;
              break;
            case STATE_IDLE:case STATE_VACATE_PENDING:case STATE_VACATED:case STATE_PREEMPTED:
            case STATE_PREEMPT_PENDING:case STATE_RESUME_PENDING:
              res = 3;
              break;
            case STATE_RUNNING:case STATE_STARTING:case STATE_COMPLETE_PENDING:
              res = 4;
              break;
            case STATE_COMPLETED:case STATE_TERMINATED: 
              res = 5;
              break;
            case STATE_REJECT_PENDING:case STATE_REMOVE_PENDING:case STATE_REJECTED:
            case STATE_CANCELED:case STATE_REMOVED:
              res = 6;
              break;
            default:
              res = 0;
              break;
          }
          if (res > 0) {
            return res;
          }
          machine = (char*)"";
          if(state==STATE_RUNNING)
          {
            holdType = 0;
            rc = ll_get_data(step, LL_StepGetFirstMachine, &machineObj);
            if(machineObj!=NULL)
            {
              rc = ll_get_data(machineObj, LL_MachineName, &machine);
            }
          } else {
            rc = ll_get_data(step, LL_StepHoldType, &holdType);
            if(holdType) { 
              return 2;
            }
          }
          ll_get_data(queryInfos, LL_JobGetNextStep, &step);
        }
      }
    }
    queryInfos = ll_next_obj(queryObject);
  }
 return 5; 
}

/**
 * \brief Function to get the start time of the job
 * \param jobId the identifier of the job 
 * \return 0 if the job is unknown or server not  unavailable
 */
time_t 
LLServer::getJobStartTime(const std::string& jobId) { 

  LL_element *queryObject;
  LL_element *queryInfos;
  LL_element *step;
  int rc;
  int objCount;
  time_t startTime;
  int errCode;

  // Set the type of query
  queryObject = ll_query(JOBS);
  if(!queryObject) {
    return 0;
  }

  char* IDlist[2];
  IDlist[0] = strdup(jobId.c_str());
  IDlist[1] = NULL;

  // Create the request
  rc = ll_set_request(queryObject, QUERY_STEPID, IDlist, ALL_DATA);

  if(rc) {
    return 0;
  }
  if(queryObject==NULL) {
    return 0;
  }

  // Calling to get the results
  queryInfos = ll_get_objs(queryObject, LL_CM, NULL, &objCount, &errCode);
  if(queryInfos==NULL) {
    return 0;
  }

  while(queryInfos)
  {
    rc = ll_get_data(queryInfos, LL_JobGetFirstStep, &step);
    if(!rc)
    {
      while(step) {
        rc = ll_get_data(step, LL_StepDispatchTime, &startTime); 
        if(!rc) {
          return startTime;
        } else {
          return 0;
        }

        ll_get_data(queryInfos, LL_JobGetNextStep, &step);
      }
    }
    queryInfos = ll_next_obj(queryObject);
  }

  return 0;
}


/**
 * \brief Function to get the number of nodes of the job
 * \param jobId the identifier of the job 
 * \return -1 if the job is unknown or server not  unavailable
 */
int 
LLServer::getJobNodeCount(const std::string& jobId) { 

  LL_element *queryObject;
  LL_element *queryInfos;
  LL_element *step;
  int rc;
  int objCount;
  int nbNodes;
  int errCode;

  // Set the type of query
  queryObject = ll_query(JOBS);
  if(!queryObject) {
    return 0;
  }

  char* IDlist[2];
  IDlist[0] = strdup(jobId.c_str());
  IDlist[1] = NULL;

  // Create the request
  rc = ll_set_request(queryObject, QUERY_STEPID, IDlist, ALL_DATA);

  if(rc) {
    return 0;
  }
  if(queryObject==NULL) {
    return 0;
  }

  // Calling to get the results
  queryInfos = ll_get_objs(queryObject, LL_CM, NULL, &objCount, &errCode);
  if(queryInfos==NULL) {
    return 0;
  }

  while(queryInfos)
  {
    rc = ll_get_data(queryInfos, LL_JobGetFirstStep, &step);
    if(!rc)
    {
      while(step) {
        rc = ll_get_data(step, LL_StepNodeCount, &nbNodes); 
        if(!rc) {
          return nbNodes;
        } else {
          return -1;
        }

        ll_get_data(queryInfos, LL_JobGetNextStep, &step);
      }
    }
    queryInfos = ll_next_obj(queryObject);
  }

  return -1;
}

/**
 * \brief Function to request the status of queues 
 * \param optQueueName (optional) the name of the queue to request 
 * \return The requested status in to ListQueues data structure 
 */
TMS_Data::ListQueues* 
LLServer::listQueues(const std::string& optQueueName) {

  LL_element *queryObject;
  LL_element  *queryInfos;
  int rc;
  int objCount;
  int errCode;
  char  *className;
  char  *classComment;
  int classPriority;
  int walltime;
  int mem;
  int maxCpu;
  int maxProcs;
  std::string errMsg;

  queryObject = ll_query(CLASSES);
  if(!queryObject) {
    errMsg = "LOADLEVELER: ll_query() return NULL.";
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errMsg);
  }

  if(optQueueName.size()==0) {
    rc = ll_set_request(queryObject, QUERY_ALL, NULL, ALL_DATA);
  } else {
    char* classList[2];
    classList[0] = strdup(optQueueName.c_str());
    classList[1] = NULL;
    rc = ll_set_request(queryObject, QUERY_CLASS, classList, ALL_DATA);
  }

  if(rc) {
    std::ostringstream oss;
    oss << "LOADLEVELER: ll_set_request() return code (" << rc << ") is non-zero."; 
    errMsg = oss.str();
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errMsg);
  }

  queryInfos = ll_get_objs(queryObject, LL_CM, NULL, &objCount, &errCode);
  if(errCode) {
    std::ostringstream oss;
    oss << "LOADLEVELER: ll_get_objs() return code (" << errCode << ") is non-zero.";
    errMsg = oss.str();
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errMsg);
  }
  if(queryInfos==NULL) {
    std::ostringstream oss;
    oss << "LOADLEVELER: ll_get_objs() return NULL.";
    errMsg = oss.str();
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errMsg);
  }

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistQueues = ecoreFactory->createListQueues();
 
  std::map<std::string, size_t> run;
  std::map<std::string, size_t> que;
  computeNbRunJobsAndQueueJobs(run, que);

  while(queryInfos)
  {

    TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();

    queue->setState(2);

    ll_get_data(queryInfos, LL_ClassName, &className);
    queue->setName(std::string(className));

    if (run.count(className)) {
      queue->setNbRunningJobs(run[className]);
    }
    if (que.count(className)) {
      queue->setNbJobsInQueue(que[className]);
    }

    ll_get_data(queryInfos, LL_ClassPriority, &classPriority);
    queue->setPriority(convertLLPrioToVishnuPrio(classPriority));

    ll_get_data(queryInfos, LL_ClassWallClockLimitSoft, &walltime);
    queue->setWallTime(walltime);

    ll_get_data(queryInfos, LL_ClassMemlockLimitHard, &mem);
    queue->setMemory(mem);

    ll_get_data(queryInfos, LL_ClassComment, &classComment);

    string description = std::string(classComment);
    size_t pos = description.find("\"");
    while(pos!=std::string::npos) {
      description.replace(pos, 1, " ");
      pos = description.find("\"");
    }

    queue->setDescription(description);  

    ll_get_data(queryInfos, LL_ClassCpuLimitHard, &maxCpu);
    queue->setMaxProcCpu(maxCpu);

    ll_get_data(queryInfos, LL_ClassMaxProcessors, &maxProcs);
    queue->setMaxJobCpu(maxProcs);    

    // Adding created queue to the list
    mlistQueues->getQueues().push_back(queue); 

    queryInfos = ll_next_obj(queryObject);

  }

  ll_free_objs(queryInfos);
  ll_deallocate(queryObject);

  mlistQueues->setNbQueues(mlistQueues->getQueues().size());
  return mlistQueues;
}

/**
 * \brief Function to compute the number of running and waiting jobs of each queue 
 * \param run contains the number of running jobs of each queue 
 * \param que contains the number of waiting jobs of each queue
 * \return raises an exception on error
 */
int 
LLServer::computeNbRunJobsAndQueueJobs(std::map<std::string, size_t>& run, 
                                       std::map<std::string, size_t>& que) {

  LL_element *queryObject;
  LL_element *queryInfos;
  LL_element *step;
  char *jclass;
  int state;
  int rc;
  int objCount;
  int errCode;
  int res = 0;
  // Set the type of query
  queryObject = ll_query(JOBS);
  if(!queryObject) {
    return -1;
  }

  // Create the request
  rc = ll_set_request(queryObject, QUERY_ALL, NULL, ALL_DATA);

  if(rc) {
    return -1;
  }
  if(queryObject==NULL) {
    return -1;
  }

  // Calling to get the results
  queryInfos = ll_get_objs(queryObject, LL_CM, NULL, &objCount, &errCode);
  if(queryInfos==NULL) {
    return -1;
  }

  while(queryInfos) {
    rc = ll_get_data(queryInfos, LL_JobGetFirstStep, &step);
    if(!rc)
    {
      while(step) {
        rc = ll_get_data(step, LL_StepJobClass, &jclass);
        if(!rc) {
          rc = ll_get_data(step, LL_StepState, &state);
          if(!rc)
          {
            switch(state) {
              case STATE_RUNNING:case STATE_STARTING:
                run[jclass]++;
                break;
              case STATE_IDLE:case STATE_PENDING:case STATE_PREEMPTED:
              case STATE_PREEMPT_PENDING:case STATE_RESUME_PENDING:case STATE_HOLD:
                que[jclass]++;
                break;
              default:
                res = 0;
                break;
            }
          } 
        }
        ll_get_data(queryInfos, LL_JobGetNextStep, &step);
      }
    }
    queryInfos = ll_next_obj(queryObject);
  }

  return res;
}

/**
 * \brief Function to convert the LL state into VISHNU state 
 * \param state the state to convert
 * \return VISHNU state 
 */
int 
LLServer::convertLLStateToVishnuState(int state) {
  int res = 0;
  switch(state) {
    case STATE_IDLE:
      res = 3;
      break;
    case STATE_RUNNING:
      res = 4;
      break;
    case STATE_STARTING:
      res = 4;
      break;
    case STATE_COMPLETE_PENDING:
      res = 4;
      break;
    case STATE_REJECT_PENDING:
      res = 6;
      break;
    case STATE_REMOVE_PENDING:
      res = 6;
      break;
    case STATE_VACATE_PENDING:
      res = 3;
      break;
    case STATE_VACATED:
      res = 3;
      break;
    case STATE_REJECTED:
      res = 6;
      break;
    case STATE_CANCELED:
      res = 6;
      break;
    case STATE_REMOVED:
      res = 6;
      break;
    case STATE_PENDING:
      res = 2;
      break;
    case STATE_PREEMPTED:
      res = 3;
      break;
    case STATE_PREEMPT_PENDING:
      res = 3;
      break;
    case STATE_RESUME_PENDING:
      res = 3;
      break;
    case STATE_COMPLETED:
      res = 5;
      break;
    case STATE_TERMINATED:
      res = 5;
      break;
    case STATE_HOLD:
      res = 2;
      break;
    case STATE_DEFERRED:
      res = 1;
      break;
    case STATE_SUBMISSION_ERR:
      res = 1;
      break;
    case STATE_NOTQUEUED:
      res = 1;
      break;
    case STATE_NOTRUN:
      res = 1;
      break;
    default:
      res = 5;
      break;
  }
  return res;
}

/**
 * \brief Function to convert the LL priority into VISHNU priority
 * \param prio the priority to convert
 * \return VISHNU state 
 */
int 
LLServer::convertLLPrioToVishnuPrio(const int& prio) {

  if(prio < -50) {
    return 1;
  } else if(prio > -50 && prio  <= 0) {
    return 2;
  } else if(prio > 0 && prio <= 50) {
    return 3;
  } else if(prio > 50 && prio <= 100) {
     return 4;
  } else if(prio > 100) {
     return 5;
  }
}

/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore 
 */
void LLServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
    const std::vector<string>& ignoredIds) {

  LL_element *queryObject;
  LL_element *queryInfos, *step, *credential;
  int rc;
  int obj_count = 0;
  int state;
  int err_code;
  int pri;
  char  *owner; 
  char* jclass;

  queryObject = ll_query(JOBS);
  if(!queryObject) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query JOBS : ll_query() return NULL.\n");
  }

  rc = ll_set_request(queryObject, QUERY_ALL, NULL, ALL_DATA);
  if(rc) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : ll_set_request() return code is non-zero.\n");
  }

  /* LL_CM : le daemon LoadL_negociator doit etre lance par le root */
  /* LL_SCHEDD : le daemon LoadL_schedd doit lance par le root */
  queryInfos = ll_get_objs(queryObject, LL_CM, NULL, &obj_count, &err_code);
  if(queryInfos==NULL) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : ll_get_objs() return code is non-zero.\n");
  }

  char *value;
  time_t submittime, wc_time_soft;
  int nbNodes = -1;;
  int nbCpu = -1;
  long nbWaitingJobs = 0;
  long nbRunningJobs = 0;

  while(queryInfos)
  {
    rc = ll_get_data(queryInfos, LL_JobCredential, &credential);
    if(!rc) {
      ll_get_data(credential, LL_CredentialUserName, &owner);
    }
    ll_get_data(queryInfos, LL_JobSubmitTime, &submittime);

    rc = ll_get_data(queryInfos, LL_JobGetFirstStep, &step);
    if(!rc)
    {
      while(step) {
        ll_get_data(step, LL_StepID, &value);
        std::vector<std::string>::const_iterator iter;
        iter = std::find(ignoredIds.begin(), ignoredIds.end(), value);
        if(iter==ignoredIds.end()) {

          TMS_Data::Job_ptr job = new TMS_Data::Job();

          job->setJobId(value);

          if(owner!=NULL) {
            job->setOwner(owner);
          }
          job->setSubmitDate(submittime);
          job->setEndDate(-1);

          ll_get_data(step, LL_StepName, &value);
          job->setJobName(value);

          ll_get_data(step, LL_StepState, &state);
          job->setStatus(convertLLStateToVishnuState(state));
          if(job->getStatus()==4) {
            nbRunningJobs++;
          } else if(job->getStatus() >= 1 && job->getStatus() <= 3) {
            nbWaitingJobs++;
          }


          ll_get_data(step, LL_StepPriority, &pri);
          job->setJobPrio(convertLLPrioToVishnuPrio(pri));

          ll_get_data(step, LL_StepOutputFile, &value);
          job->setOutputPath(value);

          ll_get_data(step, LL_StepErrorFile, &value);
          job->setErrorPath(value);

          ll_get_data(step, LL_StepWallClockLimitSoft, &wc_time_soft);
          job->setWallClockLimit(wc_time_soft);

          ll_get_data(step, LL_StepJobClass, &jclass);
          job->setJobQueue(jclass);

          ll_get_data(step, LL_StepLoadLevelerGroup, &value);
          job->setGroupName(value);

          ll_get_data(step, LL_StepComment, &value);
          job->setJobDescription(value);

          job->setMemLimit(-1);

          ll_get_data(step, LL_StepCpusPerCore, &nbCpu);
          job->setNbCpus(nbCpu);

          ll_get_data(step, LL_StepNodeCount, &nbNodes);
          job->setNbNodes(nbNodes);

          if(nbNodes!=-1) {
            job->setNbNodesAndCpuPerNode(vishnu::convertToString(nbNodes)+":"+convertToString(nbCpu));
          }
          //To fill the job working dir
          ll_get_data(step, LL_StepIwd, &value);
          job->setJobWorkingDir(value);

          listOfJobs->getJobs().push_back(job);
        }
        ll_get_data(queryInfos, LL_JobGetNextStep, &step);
      }
    }
    queryInfos = ll_next_obj(queryObject);

  }

  listOfJobs->setNbJobs(listOfJobs->getJobs().size());
  listOfJobs->setNbRunningJobs(listOfJobs->getNbRunningJobs()+nbRunningJobs);
  listOfJobs->setNbWaitingJobs(listOfJobs->getNbWaitingJobs()+nbWaitingJobs);

  ll_free_objs(queryInfos);
  ll_deallocate(queryObject);

}

/**
 * \brief Function to get the value of LoadLeveler resource (nodes, walltime, cpu) in the script
 * \param file The file contain the script to scan
 * \param optionLetterSyntax The syntax of the option (nodes, walltime, cpu) whose value will be returned
 * \return The value of the resource
 */
std::string
LLServer::getLLResourceValue(const char* file,
    const std::string& optionLetterSyntax) {

  std::string resourceValue;
  std::string LLPrefix = "#@";
  std::string line;
  ifstream ifile(file);
  if (ifile.is_open()) {
    while (!ifile.eof()) {
      getline(ifile, line);
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      size_t pos = line.find('#');
      if(pos==string::npos) {
        continue;
      }
      line = line.erase(0, pos);
      if(boost::algorithm::starts_with(boost::algorithm::erase_all_copy(line," "), LLPrefix)){
        pos = line.find("@");
        line = line.substr(pos+1);
        pos = line.find(optionLetterSyntax);
        if(pos!=std::string::npos){
          pos = line.find("=");
          if(pos!=std::string::npos){
            resourceValue = line.substr(pos+optionLetterSyntax.size()+1);
          }
        }
      }
    }

    ifile.close();
  }
  istringstream cleanResourceValue(resourceValue);
  cleanResourceValue >> resourceValue;

  return resourceValue;
}


/**
 * \brief Destructor
 */
LLServer::~LLServer() {
}
