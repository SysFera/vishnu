#include <vector>
#include <sstream>

extern "C" {
#include "pbs_ifl.h" //Torque includes
#include "pbs_error.h"
#include "cmds.h"
#include "pbs_sub.h"
#include "net_connect.h"
}

#include "TorqueServer.hpp"
#include "TMSVishnuException.hpp"
#include "utilVishnu.hpp"

const int MAX_STRING_SIZE = 255;

using namespace std;
using namespace vishnu;

TorqueServer::TorqueServer():BatchServer() {
}

int TorqueServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

  char destination[PBS_MAXDEST];
  char script_tmp[MAXPATHLEN + 1] = "";
  struct attrl *attrib= NULL;
  char errMsg[1024];
  int argc = 0;
  
  std::ostringstream os_str;

  std::vector<std::string> cmds_options;
  //processes the options
  process_options(options, cmds_options);
  argc = cmds_options.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmds_options.size(); i++) {
   argv[i+2] = const_cast<char*>(cmds_options[i].c_str());
  }

  destination[0] = '\0';
  serverOut[0] = '\0';
  //parses the scripthPath and sets the options values
  pbs_prepare_script(argc, argv, envp, script_tmp, destination, serverOut, &attrib);
  
  errMsg[0] = '\0';
  get_pbs_error_msg(errMsg);
  if(errMsg[0]!='\0') {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "TORQUE ERROR: "+std::string(errMsg));
  }

  int connect = cnt2server(serverOut);
  std::cerr << "******* connect = " << connect << std::endl;
  if (connect <= 0) {
    std::ostringstream connect_error;
    connect_error << "TORQUE ERROR: cannot connect to server ";
    connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbs_strerror(pbs_errno) << std::endl;

    if (getenv("PBSDEBUG") != NULL)
    {
      connect_error << "TORQUE ERROR: pbs_server daemon may not be running on host";
      connect_error << "or hostname in file '$TORQUEHOME/server_name' may be incorrect" << std::endl;
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());
  }
  
  pbs_errno = 0;
  char*jobId = pbs_submit(connect, (struct attropl *)attrib,
                          script_tmp, destination, NULL);

  if (jobId == NULL) {
    std::cerr << "******* jobId is NULL ...." << std::endl;
    std::ostringstream submit_error;     
    char* errmsg = pbs_geterrmsg(connect);

    if (errmsg != NULL)
    {
      submit_error << "pbs_submit: " << errmsg << std::endl;

    }
    else
    {
      submit_error << "pbs_submit: Error (" << pbs_errno << "-";
      submit_error << pbs_strerror(pbs_errno) << std::endl;
    }
    pbs_disconnect(connect);

    unlink(script_tmp);

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, submit_error.str());
  }

  pbs_disconnect(connect);
  unlink(script_tmp);

  job.setJobId(std::string(jobId));

  return 0;
}

void TorqueServer::process_options(const TMS_Data::SubmitOptions& options, std::vector<std::string>&cmds_options) {

  if(options.getName().size()!=0){
    cmds_options.push_back("-N");
    cmds_options.push_back(options.getName());
  }
  if(options.getQueue().size()!=0) {
    cmds_options.push_back("-q");
    cmds_options.push_back(options.getQueue());
  }
  if(options.getOutputPath().size()!=0) {
    cmds_options.push_back("-o");
    cmds_options.push_back(options.getOutputPath());
  }
  if(options.getErrorPath().size()!=0) {
    cmds_options.push_back("-e");
    cmds_options.push_back(options.getErrorPath());
  }
  if(options.getWallTime()!=-1) {
    cmds_options.push_back("-l"); //TODO:prendre bien en compte ce traitement dans pbs_prepare_script
    std::ostringstream os_str;
    os_str << options.getWallTime(); //TODO:A convertir en string correctement
    cmds_options.push_back("walltime="+os_str.str());
  }
  if(options.getNbCpu()!=-1) {
    cmds_options.push_back("-l");
    std::ostringstream os_str;
    //os_str.clear();
    os_str << options.getNbCpu();
    cmds_options.push_back("ncpus="+os_str.str());
  }
  if(options.getMemory()!=-1) {
    cmds_options.push_back("-l");
    //os_str.clear();
    std::ostringstream os_str;
    os_str << options.getMemory();
    cmds_options.push_back("mem="+os_str.str());
  }
  if(options.getNbNodesAndCpuPerNode()!=-1) {//TODO: le type est string
    //cmds_options.push_back("-l");
    //cmds_options.push_back("nodes=1:ppn=2");
  }

}

int TorqueServer::cancel(const char* jobId) {
  char remoteServer[MAX_STRING_SIZE];
  return pbs_cancel(jobId, remoteServer);
}

int TorqueServer::pbs_cancel(const char* jobId, char remoteServer[], bool isLocal) {

  char tmsJobId[MAX_STRING_SIZE];  
  char tmsJobIdOut[MAX_STRING_SIZE];
  char serverOut[MAX_STRING_SIZE];
  int connect;

  if(isLocal) {
    strcpy(tmsJobId, jobId);
    if (get_server(tmsJobId, tmsJobIdOut, serverOut))
    {
      std::ostringstream jobIdError;
      jobIdError << "pbs_deljob: illegally formed job identifier: " << tmsJobId << std::endl;
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,  jobIdError.str());
    }
  }

  std::cout << "tmsJobIdOut " << tmsJobIdOut << std::endl;
  std::cout << "serverOut=" << serverOut << std::endl;
  
  if(isLocal) {
     connect = cnt2server(serverOut);
  } else {
     connect = cnt2server(remoteServer);
  }

  if (connect <= 0)
  {
    std::ostringstream connect_error;
    connect_error << "TORQUE ERROR: cannot connect to server ";
    connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbs_strerror(pbs_errno) << std::endl;

    //rt_job_err("qdel", connect, tmsJobIdOut);
    
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());

  }

  pbs_errno = 0;
  int stat = pbs_deljob(connect, tmsJobIdOut, NULL);

  std::ostringstream pbs_del_error;
  if (stat && (pbs_errno != PBSE_UNKJOBID)) {
       pbs_del_error << "vishnu_cancel: " << connect << tmsJobIdOut << std::endl;
       TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, pbs_del_error.str());
       pbs_disconnect(connect);
  } else if (stat && (pbs_errno == PBSE_UNKJOBID) && isLocal ) {
    if (locate_job(tmsJobIdOut, serverOut, remoteServer)) { 
      pbs_disconnect(connect);
      pbs_cancel(tmsJobId,  remoteServer, false); 
      return 0;
    }

    pbs_del_error << "Unknown JobId " << tmsJobIdOut << std::endl;
    pbs_disconnect(connect);
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID, pbs_del_error.str());

  } else if(pbs_errno == PBSE_UNKJOBID) {
    pbs_del_error << "Unknown JobId " << tmsJobIdOut << std::endl;
    pbs_disconnect(connect);
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID, pbs_del_error.str());
  }

  pbs_disconnect(connect);
  return 0;
}

TorqueServer::~TorqueServer() { 
}

TMS_Data::ListJobs* 
TorqueServer::listJobs(TMS_Data::ListJobsOptions op){

  // The list
  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mjobs = ecoreFactory->createListJobs();

  struct attropl attr[NBFIELDLISTJOBOPT];

  char *errmsg;
  int   connect;
  struct batch_status *p_status;
  
  // Connect to the torque server
  connect = cnt2server(serverOut);

  // If connexion failure
  if (connect <= 0) {
    string msg;
    msg = "tms: cannot connect to server \n";
    
    if (getenv("PBSDEBUG") != NULL) {
      msg +=  "tms_list_jobs: pbs_server daemon may not be running on host \n";
      msg += "or hostname in file '$TORQUEHOME/server_name' may be incorrect \n";
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
  }

  // If filter on job id, no other filter is used
  if (op.getJobId()!=""){
    p_status = pbs_statjob(connect, (char *)op.getJobId().c_str(), NULL, NULL);
  } else{
    // Convert list job option to torque options
    makeListJobOption(op, attr);
    // Listing jobs
    p_status = pbs_selstat(connect, attr, NULL);
  }
  // If error listing
  if(p_status==NULL) {
    errmsg = pbs_geterrmsg(connect);
    pbs_disconnect(connect);
    throw TMSVishnuException (ERRCODE_BATCH_SCHEDULER_ERROR, string(errmsg));
  }
  // Disconnect from torque
  pbs_disconnect(connect);
 
  // Local variables to get values
  int nbJobs = 0;
  struct batch_status *p;

  // For each result
  for(p = p_status; p!=NULL; p = p->next) {
    TMS_Data::Job job;
    // Creating the corresponding job
    fillJobInfo(job, p);

    nbJobs++;

    // Adding created job to the list
    mjobs->getJobs().push_back(&job);
  } // end for
  mjobs->setNbJobs(nbJobs);
  return mjobs;
}

void TorqueServer::fill(struct attropl* attr, bool& first, char* name, char* res, char* val, enum batch_op op){
  // Last of the list
  attr[0].next = NULL;
  // If not first item of the list, attr is the next of the previous in the array
  if (!first){
    attr[-1].next = attr;
    first = false;
  }

  // Filling fields
  if (name){
    attr[0].name = (char *)malloc(sizeof(char)*strlen(name));
    strcpy(attr[0].name, name);
  }
  if (res){
    attr[0].resource = (char *)malloc(sizeof(char)*strlen(res));
    strcpy(attr[0].resource, res);
  }
  if (val){
    attr[0].value = (char *)malloc(sizeof(char)*strlen(val));
    strcpy(attr[0].value, val);
  }
  attr[0].op = op;
}

void
TorqueServer::makeListJobOption(TMS_Data::ListJobsOptions op, struct attropl* attr){
  int pos = 0;
  bool first = true;
  char* tmp;
  // If the number of cpu is an option
  if (op.getNbCpu()>0){
  fill(attr+pos, first, (char *)ATTR_l, (char *)"ncpus", (char *)convertToString(op.getNbCpu()).c_str(), EQ);
    pos++;
  }
  // If the date of submission is an option
  if (op.getFromSubmitDate()>0){
    fill(attr+pos, first, (char *)ATTR_l, (char *)"qtime", (char *)convertToString(op.getFromSubmitDate()).c_str(), GE);
    pos++;
  }
  // If the end date to submit is an option
  if (op.getToSubmitDate()>0){
    fill(attr+pos, first, (char *)ATTR_l, (char *)"qtime", (char *)convertToString(op.getToSubmitDate()).c_str(), LE);
    pos++;
  }
  // If the owner is an option
  if (op.getOwner().compare("")!=0){
    fill(attr+pos, first, (char *)ATTR_u, NULL, (char *)op.getOwner().c_str(), EQ);
    pos++;
  }
  // If the status is an option
  if (op.getStatus()>0){
    fill(attr+pos, first, (char *)ATTR_state, NULL, (char *)convertToString(op.getStatus()).c_str(), EQ);
    pos++;
  }
  // If the priority is an option
  if (op.getPriority()>0){
    fill(attr+pos, first, (char *)ATTR_p, NULL, (char *)convertToString(op.getPriority()).c_str(), EQ);
    pos++;
  }
  // If the output path is an option
  if (op.getOutPutPath().compare("")!=0){
    fill(attr+pos, first, (char *)ATTR_o, NULL, (char *)op.getOutPutPath().c_str(), EQ);
    pos++;
  }
  // If the error path is an option
  if (op.getErrorPath().compare("")!=0){
    fill(attr+pos, first, (char *)ATTR_e, NULL, (char *)op.getErrorPath().c_str(), EQ);
    pos++;
  }
  // If the queue is an option
  if (op.getQueue().compare("")!=0){
    fill(attr+pos, first, (char *)ATTR_q, NULL, (char *)op.getQueue().c_str(), EQ);
    pos++;
  }
}


void
TorqueServer::fillJobInfo(TMS_Data::Job &job, struct batch_status *p){
  struct attrl *a;
  char* trunc_str;
  size_t pos_found;
  string str;

  // Initializing all fields
  string jobid      = string("");
  string name       = string("");
  string owner      = string("");
  string timeu      = string("");
  string state      = string("");
  string location   = string("");
  string output     = string("");
  string error      = string("");
  string prio       = string("");
  string ncpus      = string("");
  string qtime      = string("");
  string group      = string("");
  string mem        = string("");
  string node       = string("");
  string wall       = string("");
  string etime      = string("");
  string nodeAndCpu = string("");
    
  // Getting job idx
  str = p->name;
  pos_found =  str.find(".");
  if(pos_found!=std::string::npos)
    pos_found =  str.find(".", pos_found+1);
  str = str.substr(0, pos_found);
  jobid = str;

  a = p->attribs;
  // Getting all the attributes of the job
  while(a!=NULL) {
    if(a->name!=NULL) {
      // Getting the value
      str = string(a->value);

      // Getting the attribute the value corresponds to
      if(!strcmp(a->name, ATTR_name)){ // job name
	name = str;
      }
      else if(!strcmp(a->name, ATTR_owner)){ // job owner
	pos_found =  string(a->value).find("@");
	str = str.substr(0, pos_found);
	owner = str;
      }
      else if(!strcmp(a->name, ATTR_used)){ // cpu time
	if(!strcmp(a->resource, "cput")) {
	  timeu = str;
	}
      }
      else if(!strcmp(a->name, ATTR_state)){ // state
	state = str;
      }
      else if(!strcmp(a->name, ATTR_queue)){ // queue
	pos_found =  std::string(a->value).find("@");
	str = str.substr(0, pos_found);
	location = str;
      }
      else if (!strcmp(a->name, ATTR_o)){ // output
	output = str;
      }
      else if (!strcmp(a->name, ATTR_e)){ // error
	error = str;
      }
      else if (!strcmp(a->name, ATTR_p)){ // priority
	prio = str;
      }
      else if (!strcmp(a->name, ATTR_l)){ // nbcpu or qtime
	if (!strcmp(a->resource, "ncpus")){
	  ncpus = str;
	}
	else if(!strcmp(a->resource, "qtime")){
	  qtime = str;
	}
	else if(!strcmp(a->resource, "mem")){
	  mem = str;
	}
	else if(!strcmp(a->resource, "walltime")){
	  wall = str;
	}
	else if(!strcmp(a->resource, "nodes")){ // node and nodeandcpupernode
	  string tmp;
	  pos_found =  string(a->value).find(":");
	  if (pos_found != string::npos){
	    tmp = str.substr(0, pos_found);
	    node = tmp;
	    nodeAndCpu = str;
	  }
	  else{
	    node = str;
	  }
	}
      }
      else if (!strcmp(a->name, ATTR_g)){ // group
	group = str;
      }
      else if (!strcmp(a->name, ATTR_etime)){ // end time ?
	etime = str;
      }	      
      a = a->next;
    }// end if name != null
  } // end while
    // TODO :
    // JOBPATH ?
    // SUBMITMACHINEID ?
    // SESSIONID ?
    // SCRIPCONTENT ?
    // WORKING DIR ?
    // DESCRIPTION ?
    //
    // READ FROM DATABASE BUT IN A HIGHER LEVEL
    //

    // Creating job
  job.setJobId(jobid);
  job.setJobName(name);
  job.setOwner(owner);
  //      job.setJobId(timeu); ? timeu ? TODO
  if (state.compare("")!=0)
    job.setStatus(atoi(state.c_str()));
  else
    job.setStatus(0);
  job.setJobQueue(location);
  job.setOutputPath(output);
  job.setErrorPath(error);
  if (prio.compare("")!=0)
    job.setJobPrio(atoi(prio.c_str()));
  else
    job.setJobPrio(0);
  if (ncpus.compare("")!=0)
    job.setNbCpus(atoi(ncpus.c_str()));
  else
    job.setNbCpus(0);
  if (qtime.compare("")!=0)
    job.setSubmitDate(atol(qtime.c_str()));
  else
    job.setSubmitDate(0);
  job.setGroupName(group);
  if (etime.compare("")!=0)
    job.setEndDate(atol(etime.c_str()));
  else
    job.setEndDate(0);
  if (wall.compare("")!=0)
    job.setWallClockLimit(atol(wall.c_str()));
  else
    job.setWallClockLimit(0);
  if (mem.compare("")!=0)
    job.setMemLimit(atoi(mem.c_str()));
  else
    job.setMemLimit(0);
  if (node.compare("")!=0)
    job.setNbNodes(atoi(node.c_str()));
  else
    job.setNbNodes(0);
  // TODO uncomment once api corrected
  //
  //    job.setNbNodesAndCpuPerNode(nodeAndCpu);
}


void
TorqueServer::fillJobProgress(TMS_Data::Progression &job, struct batch_status *p){
  struct attrl *a;
  char* trunc_str;
  size_t pos_found;
  string str;

  // Initializing all fields
  string jobid      = string("");
  string name       = string("");
  string state      = string("");
  string wall       = string("");
  string etime      = string("");
  string stime      = string("");
  string nodeAndCpu = string("");
  string cput       = string("");

  // Getting job idx
  str = p->name;
  pos_found =  str.find(".");
  if(pos_found!=std::string::npos)
    pos_found =  str.find(".", pos_found+1);
  str = str.substr(0, pos_found);
  jobid = str;

  a = p->attribs;
  // Getting all the attributes of the job
  while(a!=NULL) {
    if(a->name!=NULL) {
      // Getting the value
      str = string(a->value);

      // Getting the attribute the value corresponds to
      if(!strcmp(a->name, ATTR_name)){ // job name
	name = str;
      }
      else if(!strcmp(a->name, ATTR_state)){ // state
	state = str;
      }
      else if (!strcmp(a->name, ATTR_l)){ // nbcpu or qtime
	if(!strcmp(a->resource, "walltime")){
	  wall = str;
	}
	if(!strcmp(a->resource, "cput")){
	  cput = str;
	}
      }
      else if (!strcmp(a->name, ATTR_etime)){ // end time ?
	etime = str;
      }
      else if (!strcmp(a->name, ATTR_start)){ // start time ?
	stime = str;
      }	      
      a = a->next;
    }// end if name != null
  } // end while

  double start, end, percent;

  // Creating job
  job.setJobId(jobid);
  job.setJobName(name);
  if (state.compare("")!=0)
    job.setStatus(atoi(state.c_str()));
  else
    job.setStatus(0);

  end = double(atol(etime.c_str()));
  job.setEndTime(end);

  start = double(atol(stime.c_str()));
  job.setStartTime(start);

  if (wall.compare("")!=0)
    job.setWallTime(atol(wall.c_str()));
  else
    job.setWallTime(0);

  job.setPercent(atoi(cput.c_str())/atoi(wall.c_str()));
}

TMS_Data::ListProgression*
TorqueServer::getJobProgress(TMS_Data::ProgressOptions op){

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mprog = ecoreFactory->createListProgression();

  struct attropl attr[NBFIELDLISTJOBOPT];

  char *errmsg;
  int   connect;
  struct batch_status *p_status;
  
  // Connect to the torque server
  connect = cnt2server(serverOut);

  // If connexion failure
  if (connect <= 0) {
    string msg;
    msg = "tms: cannot connect to server \n";
    
    if (getenv("PBSDEBUG") != NULL) {
      msg +=  "tms_list_jobs: pbs_server daemon may not be running on host \n";
      msg += "or hostname in file '$TORQUEHOME/server_name' may be incorrect \n";
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
  }

  // If jobid given, the info about this job are gotten
  if (op.getJobId()!=""){
    p_status = pbs_statjob(connect, (char *)op.getJobId().c_str(), NULL, NULL);
  } else{
    // Stat for a job only
    bool b=true;
    fill(attr, b, (char *)ATTR_u, NULL, (char *)op.getJobOwner().c_str(), EQ);

    // Listing jobs
    p_status = pbs_selstat(connect, attr, NULL);
  }
  // If error listing
  if(p_status==NULL) {
    errmsg = pbs_geterrmsg(connect);
    pbs_disconnect(connect);
    throw TMSVishnuException (ERRCODE_BATCH_SCHEDULER_ERROR, string(errmsg));
  }
  // Disconnect from torque
  pbs_disconnect(connect);
 
  // Local variables to get values
  int nbJobs = 0;
  struct batch_status *p;

  // For each result
  for(p = p_status; p!=NULL; p = p->next) {
    TMS_Data::Progression job;
    // Creating the corresponding progression
    fillJobProgress(job, p);

    nbJobs++;

    // Adding created job to list
    mprog->getProgress().push_back(&job);
  } // end for
  mprog->setNbJobs(nbJobs);
  return mprog;  
}


TMS_Data::Job* 
TorqueServer::getJobInfo(string job){

  // The list
  // TODO: Check the new, because memor leaks otherwise
  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mjob = ecoreFactory->createJob();

  char *errmsg;
  int   connect;
  struct batch_status *p_status;
  
  // Connect to the torque server
  connect = cnt2server(serverOut);

  // If connexion failure
  if (connect <= 0) {
    string msg;
    msg = "tms: cannot connect to server \n";
    
    if (getenv("PBSDEBUG") != NULL) {
      msg +=  "tms_list_jobs: pbs_server daemon may not be running on host \n";
      msg += "or hostname in file '$TORQUEHOME/server_name' may be incorrect \n";
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
  }

  // If filter on job id, no other filter is used
  p_status = pbs_statjob(connect, (char *)job.c_str(), NULL, NULL);

  // If error listing
  if(p_status==NULL) {
    errmsg = pbs_geterrmsg(connect);
    pbs_disconnect(connect);
    throw TMSVishnuException (ERRCODE_BATCH_SCHEDULER_ERROR, string(errmsg));
  }
  // Disconnect from torque
  pbs_disconnect(connect);
 
  // Local variables to get values
  int nbJobs = 0;
  struct batch_status *p = p_status;

  // Creating the corresponding job
  fillJobInfo(*mjob, p);

  return mjob;
}
