#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "llapi.h"
#include "LLServer.hpp"
#include "TMSVishnuException.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;
using namespace std;

LLServer::LLServer():BatchServer() {
}

int LLServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

  /*LL_job llJobInfo;
  //options // TODO: A prendre en compte
  if(llsubmit(const_cast<char*>(scriptPath), NULL, NULL, &llJobInfo, LL_JOB_VERSION)) {
    return -1 ;//TODO: A remplacer par l'envoie d'une exception
  };

  std::ostringstream llJobId;
  llJobId<< (llJobInfo.step_list[0])->id.from_host;
  llJobId<< "." << (llJobInfo.step_list[0])->id.cluster;
  llJobId<< "." << (llJobInfo.step_list[0])->id.proc;
  
  job.setJobId(llJobId.str());

  llfree_job_info(&llJobInfo,LL_JOB_VERSION);
  return 0;*/
  
  /**Test sur la machine distante blue gene*/
  return remove_test(job);

}

int LLServer::cancel(const char* jobId) { 

  std::ostringstream cmd;
  //std::string  cancelCommand="llcancel"; //The correct value
  std::string  cancelCommand="ssh bg llcancel";
  
  cmd << cancelCommand << " " << jobId;
  if(system((cmd.str()).c_str())) { //A remplacer par excec
    std::cerr << "LLServer::cancel: can't cancel this job" << std::endl;
    return -1; //Une exception a lever ici
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
  // If filter on output and invalid output
  if (op.getOutPutPath().compare("")!=0 && it->getOutputPath() != op.getOutPutPath()){
    return true;
  }
  // If filter on error and invalid error
  if (op.getErrorPath().compare("")!=0 && it->getErrorPath() != op.getErrorPath()){
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
  int rc, obj_count, state, err_code, hold_type, pri;
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
  queryInfos = ll_get_objs(queryObject, /*LL_SCHEDD*/LL_CM, NULL, &obj_count, &err_code);
  if(queryInfos==NULL) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : ll_get_objs() returns NULL.\n");
  }


  int nb_jobs = obj_count;
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
	    job.setNbNodesAndCpuPerNode(cpuPcore);

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

