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
  job.setOutputPath(std::string((llJobInfo.step_list[0])->out)) ;
  job.setErrorPath(std::string((llJobInfo.step_list[0])->err));
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
  //job.setNbNodesAndCpuPerNode(llJobInfo.step_list[0]->limits.cpus_per_core); 

  llfree_job_info(&llJobInfo,LL_JOB_VERSION);

  return 0;

  /**Test sur la machine distante blue gene aux Etats-Unis*/
  //return remove_test(job);

}

int LLServer::cancel(const char* jobId) { 

  std::ostringstream cmd;
  std::string  cancelCommand="llcancel"; //The correct value
  //std::string  cancelCommand="ssh bg llcancel";// pour des tests sur la machine blue aux Etats-Unis
  
  cmd << cancelCommand << " " << jobId;
  if(system((cmd.str()).c_str())) { //A remplacer par excec
    std::cerr << "LLServer::cancel: can't cancel this job" << std::endl;
    return -1; //Une exception a lever ici
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
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query JOBS : ll_query() return NULL.\n");
  }

  char* IDlist[2];
  IDlist[0] = strdup(jobId.c_str());
  IDlist[1] = NULL;

  bool isStepId = false;
  size_t found = std::string(IDlist[0]).find(".");
  if(found!=std::string::npos)
  {
    found = std::string(IDlist[0]).find(".", found+1);
    if(found!=std::string::npos) {
      isStepId = true;
    }
  }

  // Create the request
  if(!isStepId) {
    rc = ll_set_request(queryObject, QUERY_JOBID, IDlist, ALL_DATA);
  }
  else {
    rc = ll_set_request(queryObject, QUERY_STEPID, IDlist, ALL_DATA);
  }

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
              return 3;
            case STATE_REMOVE_PENDING:
              return 6;
            case STATE_VACATE_PENDING:
              return 6;
            case STATE_VACATED:
              return 6;
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
  } //while(queryInfos)

 return -1; 
}

time_t getJobProgressInfo(const std::string& jobId) { 

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

  bool isStepId = false;
  size_t found = std::string(IDlist[0]).find(".");
  if(found!=std::string::npos)
  {
    found = std::string(IDlist[0]).find(".", found+1);
    if(found!=std::string::npos) {
      isStepId = true;
    }
  }

  // Create the request
  if(!isStepId) {
    rc = ll_set_request(queryObject, QUERY_JOBID, IDlist, ALL_DATA);
  }
  else {
    rc = ll_set_request(queryObject, QUERY_STEPID, IDlist, ALL_DATA);
  }

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
        rc = ll_get_data(step, LL_StepStartTime, &startTime); 
        if(!rc) {
          return startTime;
        } else {
          return 0;
        }

        ll_get_data(queryInfos, LL_JobGetNextStep, &step);
      }
    }
  }

  return 0;
}

int LLServer::remove_test(TMS_Data::Job& job) {
  /**Test sur la machine distante blue gene*/
  std::ostringstream cmd;
  std::string  submitCommand="ssh bg /u/home/ac/ecaron/TEST/APILoadLevelerTest/submittest";
  std::string scriptTest = "/u/home/ac/ecaron/TEST/APILoadLevelerTest/Loadleveler_script2";
  cmd << submitCommand << " " << scriptTest;
  cmd << " > testLL.jobId";

  if(system((cmd.str()).c_str())) { //a remplacer par excec
    std::cerr << "llserver::submit: can't submit the script" << std::endl;
    return -1; //une exception a lever ici  
  }

  std::string jobId;
  ifstream ifile;
  ifile.open("testLL.jobId");

  getline(ifile, jobId);
  getline(ifile, jobId);

  ifile.close();

  if(jobId.size()!=0) {
    std::cout << jobId << std::endl;
    size_t pos = jobId.find(":");
    size_t size = jobId.size();
    jobId = jobId.substr(pos+1, size-pos);
    job.setJobId(jobId);
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

  while(queryInfos)
  {

    TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();
    queue->setWallTime(-1);
    queue->setMemory(-1);
    queue->setNode(-1);
    queue->setNbRunningJobs(0);
    queue->setNbJobsInQueue(0);
    queue->setMaxProcCpu(-1);
    queue->setMaxJobCpu(-1);

    ll_get_data(queryInfos, LL_ClassName, &className);
    queue->setName(std::string(className));

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

    // Adding created job to the list
    mlistQueues->getQueues().push_back(queue); 

    queryInfos = ll_next_obj(queryObject);

  }

  ll_free_objs(queryInfos);
  ll_deallocate(queryObject);

  mlistQueues->setNbQueues(mlistQueues->getQueues().size());
  return mlistQueues;
}

TMS_Data::ListJobs*
LLServer::listJobs(TMS_Data::ListJobsOptions op){
  // Listing all jobs
  try{
    mjobs = listAllJobs();
  }catch (TMSVishnuException& e){
    throw e;
  }
  // If filter on the job id
  if(op.getJobId().compare("")!=0){
    try{
      mjob = filterJob(op.getJobId());
    }
    catch(TMSVishnuException e){
      throw e;
    }
    // Create a list with only the found job
    mjobs->getJobs().clear();
    mjobs->getJobs().push_back(mjob);
    return mjobs;
  }
  // Otherwize apply filter (options) on the list
  mjobs = filterJobs(op);
  return mjobs;
}

TMS_Data::ListJobs*
LLServer::filterJobs(TMS_Data::ListJobsOptions op){
  // Copy because cannot remove from the ecore list
  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  TMS_Data::ListJobs_ptr tmp;
  tmp = TMS_Data::ListJobs_ptr(mjobs);

  // Empty mjobs
  mjobs = ecoreFactory->createListJobs();

  // Fill mjobs with the rights jobs
  for (unsigned int i = 0 ; i < mjobs->getJobs().size() ; i++) {
    if (!notIn (mjobs->getJobs().get(i), op)) {
      mjobs->getJobs().push_back(mjobs->getJobs().get(i));
    }
  }
  return mjobs;
}

bool 
LLServer::notIn(TMS_Data::Job_ptr it, TMS_Data::ListJobsOptions op){
  // If filter on nb cpu and invalid number of cpu
  if (op.getNbCpu()>0 && it->getNbCpus() != op.getNbCpu()){
    return true;
  }
  // If filter on submit date and invalid submit date
  if (op.getFromSubmitDate()>0 && it->getSubmitDate() < op.getFromSubmitDate()){
    return true;
  }
  // If filter end date and invalid end date
  if (op.getToSubmitDate()>0 && it->getSubmitDate() > op.getToSubmitDate()){
    return true;
  }
  // If filter on owner and invalid owner
  if (op.getOwner().compare("")!=0 && it->getOwner() != op.getOwner()){
    return true;
  }
  // If filter on status and invalid status
  if (op.getStatus()>0 && it->getStatus() != op.getStatus()){
    return true;
  }
  // If filter on priority and invalid priority
  if (op.getPriority()>0 && it->getJobPrio() != op.getPriority()){
    return true;
  }
  // If filter on queue and invalid queue
  if (op.getQueue().compare("")!=0 && it->getJobQueue() != op.getQueue()){
    return true;
  }
  return false;
}


TMS_Data::Job*
LLServer::filterJob(string jobid){
  // For each job
  for (unsigned int i = 0 ; i < mjobs->getJobs().size() ; i++){
    // If its job id corresponds
    if (mjobs->getJobs().get(i)->getJobId().compare(jobid)==0){
      // Return it
      mjob = mjobs->getJobs().get(i);
      return mjob;
    }
  }
  // Throw an exception if the job is not found
  throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Job not found with the jobid: "+jobid);
  return NULL;
}


TMS_Data::ListJobs*
LLServer::listAllJobs(){
  // The list
  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mjobs = ecoreFactory->createListJobs();
   
  LL_job job_info;
  LL_element *queryObject, *queryInfos, *step, *credential, *machine_obj;
  int rc, objCount, state, errCode, hold_type, pri;
  char  *owner, *machine, *id, *jclass;
  time_t submittime;
  time_t endtime;
  char* state_tab[] = {(char*)"R", (char*)"I", (char*)"NQ", (char*)"E", (char*)"H"} ;
  char* state2str;
  struct tm *date_tm;

  char* name;
  char* output;
  char* error;
  char* wall;
  char* group;
  char* iwd; // Initial Working Directory
  int 	cpuPcore;
  int 	queueId;
  int 	mem;
  int 	cpu;
  int   node;

  // Set the type of query
  queryObject = ll_query(JOBS);
  if(!queryObject) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query JOBS : ll_query() return NULL.\n");
  }

  // Create the request  
  rc = ll_set_request(queryObject, QUERY_ALL, NULL, ALL_DATA);
  if(rc) {
    string msg = "Query Infos : ll_set_request() return code is non-zero: "+convertToString(rc)+"\n";
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
  }
  if(queryObject==NULL) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "querryObject is NULL");
  }

  // Calling to get the results
  queryInfos = ll_get_objs(queryObject, /*LL_SCHEDD*/LL_CM, NULL, &objCount, &errCode);
  if(queryInfos==NULL) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : ll_get_objs() returns NULL.\n");
  }


  int nb_jobs = objCount;
  // Transforming each result in a TMS_Data::Job
  while(queryInfos)
    {

      rc = ll_get_data(queryInfos, LL_JobCredential, &credential);
      if(!rc) {
	// Owner of job
	ll_get_data(credential, LL_CredentialUserName, &owner);
	// Group of submission
	ll_get_data(credential, LL_CredentialGroupName, &group);
      }
      else {
	throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the credential.\n");
      }

      // Name of job
      rc = ll_get_data(queryInfos, LL_JobName, &name);
      if (rc){
	throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the job name.\n");
      }
      // Submit time of job
      rc = ll_get_data(queryInfos, LL_JobSubmitTime, &submittime);
      if(rc) {
	throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the time the job was submitted.\n");
      }
      // Getting step
      rc = ll_get_data(queryInfos, LL_JobGetFirstStep, &step);
      if(!rc) {

	// For each step, a step is a job
	while(step) {

	  // Output file
	  rc = ll_get_data(step, LL_StepOutputFile, &output);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the output file.\n");
	  }
	  // Error file
	  rc = ll_get_data(step, LL_StepErrorFile, &error);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the error file.\n");
	  }
	  //  Wall
	  rc = ll_get_data(step, LL_StepWallClockUsed, &wall);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the wall time.\n");
	  }
	  // Initial Working Directory
	  rc = ll_get_data(step, LL_StepIwd, &iwd);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the initial working directory.\n");
	  }
	  // Cpu per Core
	  rc = ll_get_data(step, LL_StepCpusPerCore, &cpuPcore);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the cpu per core.\n");
	  }
	  // Queue Id
	  rc = ll_get_data(step, LL_StepQueueId, &queueId);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the queue id.\n");
	  }
	  // Nb node
	  rc = ll_get_data(step, LL_StepNodeUsage, &node);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the queue id.\n");
	  }
	  // End date
	  rc = ll_get_data(step, LL_StepCompletionDate, &endtime);
	  if (rc){
	    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the end time.\n");
	  }
	  // State
	  rc = ll_get_data(step, LL_StepState, &state);
	  if(!rc) {
	    if(state==STATE_IDLE) state2str = state_tab[1];
	    if(state==STATE_RUNNING)  state2str = state_tab[0];
	    if(state==STATE_PENDING)  state2str = state_tab[2];
	    if(state==STATE_PREEMPTED) state2str = state_tab[3];

	    machine = (char *)"";
	    if(state==STATE_RUNNING) {
	      hold_type = 0;
	      rc = ll_get_data(step, LL_StepGetFirstMachine, &machine_obj);
	      if (rc){
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the machine object.\n");
	      }
	      if(machine_obj!=NULL){
		// Machine name
		rc = ll_get_data(machine_obj, LL_MachineName, &machine);
		if (rc){
		  throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the machine.\n");
		}
		// Nb CPU on machine
		rc = ll_get_data(machine_obj, LL_MachineCPUs, &cpu);
		if (rc){
		  throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the number of cpu.\n");
		}
		// Memory on machine
		rc = ll_get_data(machine_obj, LL_MachineRealMemory, &mem);
		if (rc){
		  throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the memory.\n");
		}

	      }
	    } else {
	      rc = ll_get_data(step, LL_StepHoldType, &hold_type);
	      if (rc){
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the hold tipe.\n");
	      }
	      if(hold_type)  state2str = state_tab[4];
	    }
	    // Job id
	    rc=ll_get_data(step, LL_StepID, &id);
	    if(rc) {
	      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the id\n");
	    }
	    // Priority
	    rc=ll_get_data(step, LL_StepPriority, &pri);
	    if (rc){
	      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the priority.\n");
	    }
	    // Job class
	    rc=ll_get_data(step, LL_StepJobClass, &jclass);
	    if (rc){
	      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : Error getting the job class.\n");
	    }
	    // Creating the corresponding job
	    TMS_Data::Job job;
	    job.setJobId(id);
	    job.setJobName(name);
	    job.setOutputPath(output);
	    job.setErrorPath(error);
	    job.setJobPrio(pri);
	    job.setNbCpus(cpu);
	    job.setJobWorkingDir(iwd);
	    job.setStatus(state);
	    job.setSubmitDate(submittime);
	    job.setEndDate(endtime);
	    job.setOwner(owner);
	    // TODO : check queue
	    job.setJobQueue(convertToString(queueId));
	    job.setWallClockLimit(atol(wall));
	    job.setGroupName(group);
	    job.setMemLimit(mem);
	    job.setNbNodes(node);

	    mjobs->getJobs().push_back(&job);

	    // Next step
	    ll_get_data(queryInfos, LL_JobGetNextStep, &step);
	  }
	}
      }
      queryInfos = ll_next_obj(queryObject);
    }
  
  ll_free_objs(queryInfos);
  ll_deallocate(queryObject);

  return mjobs;
}


TMS_Data::Job* 
LLServer::getJobInfo(string job){
  // Listing all jobs
  try{
    mjobs = listAllJobs();
  }catch (TMSVishnuException& e){
    throw e;
  }
  try{
    mjob = filterJob(job);
  }
  catch(TMSVishnuException e){
    throw e;
  }
  return mjob;
}

TMS_Data::ListProgression*
LLServer::getJobProgress(TMS_Data::ProgressOptions op){
  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mprog = ecoreFactory->createListProgression();
  TMS_Data::Progression_ptr prog;
  // Listing all jobs
  try{
    mjobs = listAllJobs();
  }catch (TMSVishnuException& e){
    throw e;
  }
  // If filter on the job id
  if(op.getJobId().compare("")!=0){
    try{
      mjob = filterJob(op.getJobId());
      // Create the progression object
      makeProg(prog, mjob);
      // Create a list with only the found job
      mprog->getProgress().push_back(prog);
    }
    catch(TMSVishnuException e){
      throw e;
    }
  }
  else if (op.getJobOwner().compare("")!=0) {
    TMS_Data::ListJobsOptions jobOp;
    jobOp.setOwner(op.getJobOwner());
    mjobs = filterJobs(jobOp);
    for (unsigned int i = 0 ; i < mjobs->getJobs().size() ; i++){
      makeProg(prog, mjobs->getJobs().get(i));
      mprog->getProgress().push_back(prog);
    }
  }
  else {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "No filter given to get progression ");    
  }
  return mprog;
}


void
LLServer::makeProg(TMS_Data::Progression_ptr prog, TMS_Data::Job_ptr job){
  time_t current; // TODO : FILL CURRENT
  //LL_StepWallClockUsed A utiliser
  double percent = (current-job->getSubmitDate())/(job->getWallClockLimit()-job->getSubmitDate());
  prog->setJobId(job->getJobId());
  prog->setJobName(job->getJobName());
  prog->setWallTime(job->getWallClockLimit());
  prog->setStartTime(job->getSubmitDate());
  prog->setEndTime(job->getEndDate());
  prog->setStatus(job->getStatus());
  prog->setPercent(percent);
}
