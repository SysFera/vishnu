#include <string>
#include <iostream>
#include <sstream>
#include "llapi.h"

#include "LLServer.hpp"

#include "TMSVishnuException.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;
using namespace std;

LLServer::LLServer():BatchServer() {
 mlistQueues = NULL;
}

int LLServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

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
  job.setStatus(llJobInfo.step_list[0]->status);//A mapper en VISHNU status
  job.setJobName(std::string(llJobInfo.job_name));
  job.setSubmitDate((llJobInfo.step_list[0])->q_date);
  job.setOwner(std::string(llJobInfo.owner));
  job.setJobQueue(std::string((llJobInfo.step_list[0])->stepclass));
  job.setWallClockLimit(llJobInfo.step_list[0]->limits.soft_wall_clock_limit);
  job.setEndDate(-1);
  job.setGroupName(std::string((llJobInfo.step_list[0])->group_name));
  job.setJobDescription(std::string((llJobInfo.step_list[0])->comment));
  job.setJobPrio((llJobInfo.step_list[0])->prio);//WARNING: a convertir en VISHNU PRIORITY TYPE
  job.setMemLimit(llJobInfo.step_list[0]->limits64.memlock_soft_limit);
  job.setNbCpus(llJobInfo.step_list[0]->limits.cpu_soft_limit);
  job.setNbNodes(llJobInfo.step_list[0]->limits.core_soft_limit);

  llfree_job_info(&llJobInfo,LL_JOB_VERSION);

  return 0;
}

int LLServer::cancel(const char* jobId) { 

  std::ostringstream cmd;
  std::string  cancelCommand="llcancel"; //The correct value
  
  cmd << cancelCommand << " " << jobId;
  if(system((cmd.str()).c_str())) { //A remplacer par excec
    std::cerr << "LLServer::cancel: can't cancel this job" << std::endl;
    return -1;;//error messages are written to stderr, VISHNU redirects these messages into a file
  }
 
 return 0; 
}

int LLServer::processOptions(const char* scriptPath, const TMS_Data::SubmitOptions& options) {

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
      optionLineToInsert ="# @ wall_clock_limit="+vishnu::convertWallTimeToString(options.getWallTime());
      insertOptionLine(optionLineToInsert, content);
    }


    if(optionLineToInsert.size()!=0) {
      ofstream ofs(scriptPath);
      ofs << content;
      ofs.close();
    }
  }

}

int LLServer::insertOptionLine(const std::string& optionLineToInsert, std::string& content) {

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
        std::cout << line << std::endl;
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

int LLServer::getJobState(const std::string& jobId) {
 
  LL_job jobInfo;
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
    return -1;
  }

  char* IDlist[2];
  IDlist[0] = strdup(jobId.c_str());
  IDlist[1] = NULL;

  // Create the request
  rc = ll_set_request(queryObject, QUERY_STEPID, IDlist, ALL_DATA);

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

  while(queryInfos)
  {

    rc = ll_get_data(queryInfos, LL_JobGetFirstStep, &step);
    if(!rc)
    {
      while(step) {
        rc = ll_get_data(step, LL_StepState, &state);
        if(!rc)
        {
          switch(state) {
            case STATE_IDLE:
              return 3; 
            case STATE_RUNNING:
              return 4;
            case STATE_STARTING:
              return 4;
            case STATE_COMPLETE_PENDING:
              return 4;
            case STATE_REJECT_PENDING:
              return 6;
            case STATE_REMOVE_PENDING:
              return 6;
            case STATE_VACATE_PENDING:
              return 3;
            case STATE_VACATED:
              return 3;
            case STATE_REJECTED:
              return 6;
            case STATE_CANCELED:
              return 6;
            case STATE_REMOVED:
              return 6;
            case STATE_PENDING:
              return 2 ;
            case STATE_PREEMPTED:
              return 3;
            case STATE_PREEMPT_PENDING:
              return 3;
            case STATE_RESUME_PENDING:
              return 3;
            case STATE_COMPLETED: 
              return 5;
            case STATE_TERMINATED:
              return 5;
            case STATE_HOLD:
              return 2;
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
        } //end if(!rc)
      } // end while(step)
    } // end if(!rc)
    queryInfos = ll_next_obj(queryObject);
  } //while(queryInfos)

 return -1; 
}

time_t LLServer::getJobStartTime(const std::string& jobId) { 

  LL_job jobInfo;
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

TMS_Data::ListQueues* LLServer::listQueues(const std::string& optQueueName) {

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
 
  std::map<std::string, int> run;
  std::map<std::string, int> que;
  computeNbRunJobsAndQueueJobs(run, que);

  while(queryInfos)
  {

    TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();

    ll_get_data(queryInfos, LL_ClassName, &className);
    queue->setName(std::string(className));

    if (run.count(className)) {
      queue->setNbRunningJobs(run[className]);
    }
    if (que.count(className)) {
      queue->setNbJobsInQueue(que[className]);
    }

    ll_get_data(queryInfos, LL_ClassPriority, &classPriority);
    queue->setPriority(classPriority);

    ll_get_data(queryInfos, LL_ClassWallClockLimitSoft, &walltime);
    queue->setWallTime(walltime);

    ll_get_data(queryInfos, LL_ClassMemlockLimitHard, &mem);
    queue->setMemory(mem);

    ll_get_data(queryInfos, LL_ClassComment, &classComment);
    queue->setDescription(classComment);  

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

int LLServer::computeNbRunJobsAndQueueJobs(std::map<std::string, int>& run, std::map<std::string, int>& que) {

  LL_element *queryObject;
  LL_element *queryInfos;
  LL_element *step;
  char *jclass;
  int state;
  int rc;
  int objCount;
  int errCode;
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
              case STATE_RUNNING:
                run[jclass]++;
                break;
              case STATE_IDLE:
                que[jclass]++;
                break;
              case STATE_STARTING:
                run[jclass]++;
                break;
              case STATE_PENDING:
                que[jclass]++;
                break;
              case STATE_PREEMPTED:
                que[jclass]++;
                break;
              case STATE_PREEMPT_PENDING:
                que[jclass]++;
                break;
              case STATE_RESUME_PENDING:
                que[jclass]++;
                break;
              case STATE_HOLD:
                que[jclass]++;
                break;
            }
          } 
        }

        ll_get_data(queryInfos, LL_JobGetNextStep, &step);
      }
    }
    queryInfos = ll_next_obj(queryObject);
  }

  return 0;
}

LLServer::~LLServer() {
}
