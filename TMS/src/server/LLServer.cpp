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

LLServer::LLServer():BatchServer() {
}

int LLServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

  LL_job llJobInfo;
  //options // TODO: A prendre en compte
  if(llsubmit(const_cast<char*>(scriptPath), NULL, NULL, &llJobInfo, LL_JOB_VERSION)) {
    return -1 ;//TODO: A remplacer par l'envoie d'une exception
  };

  std::ostringstream llJobId;
  llJobId<< (llJobInfo.step_list[0])->id.from_host;
  llJobId<< (llJobInfo.step_list[0])->id.cluster;
  llJobId<< (llJobInfo.step_list[0])->id.proc;
  
  job.setJobId(llJobId.str());

  llfree_job_info(&llJobInfo,LL_JOB_VERSION);
}

int cancel(const char* jobId) { 

  std::ostringstream cmd;
  std::string  cancelCommand="/opt/ibmll/LoadL/full/bin/llcancel";
  
  cmd << cancelCommand << " " << jobId;
  if(system((cmd.str()).c_str())) { //A remplacer par excec
    return -1; //Une exception a lever ici  
  }
 
 return 0; 
}

TMS_Data::ListJobs*
LLServer::listJobs(TMS_Data::ListJobsOptions op){
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
  
  rc = ll_set_request(queryObject, QUERY_ALL, NULL, ALL_DATA);
  if(rc) {
    string msg = "Query Infos : ll_set_request() return code is non-zero: "+convertToString(rc)+"\n";
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
  }
  
  if(queryObject==NULL) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "querryObject is NULL");
  }

  queryInfos = ll_get_objs(queryObject, /*LL_SCHEDD*/LL_CM, NULL, &obj_count, &err_code);
  if(queryInfos==NULL) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Query Infos : ll_get_objs() returns NULL.\n");
  }


  int nb_jobs = obj_count;

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
	// For each step
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

	    // Next step
	    ll_get_data(queryInfos, LL_JobGetNextStep, &step);
	  }
	}
      }
      queryInfos = ll_next_obj(queryObject);
    }
  
  ll_free_objs(queryInfos);
  ll_deallocate(queryObject);
  
}
