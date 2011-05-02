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
  serverOut[0] = '\0'; //serveur par defaut
}

int TorqueServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

  char destination[PBS_MAXDEST];
  char scriptTmp[MAXPATHLEN + 1] = "";
  struct attrl *attrib= NULL;
  char errMsg[1024];
  int argc = 0;
  
  std::ostringstream os_str;

  std::vector<std::string> cmdsOptions;
  //processes the options
  processOptions(options, cmdsOptions);
  argc = cmdsOptions.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmdsOptions.size(); i++) {
   argv[i+2] = const_cast<char*>(cmdsOptions[i].c_str());
  }

  destination[0] = '\0';
  serverOut[0] = '\0';
  //parses the scripthPath and sets the options values
  pbs_prepare_script(argc, argv, envp, scriptTmp, destination, serverOut, &attrib);
  
  errMsg[0] = '\0';
  get_pbs_error_msg(errMsg);
  if(errMsg[0]!='\0') {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "TORQUE ERROR: "+std::string(errMsg));
  }

  int connect = cnt2server(serverOut);
  std::cerr << "******* connect = " << connect << std::endl;
  if (connect <= 0) {
    std::ostringstream connect_error;
    connect_error << "TORQUE ERROR: pbs_submit: cannot connect to server ";
    connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbs_strerror(pbs_errno) << std::endl;

    if (getenv("PBSDEBUG") != NULL)
    {
      connect_error << "TORQUE ERROR: pbs_submit: pbs_server daemon may not be running on host";
      connect_error << "or hostname in file '$TORQUEHOME/server_name' may be incorrect" << std::endl;
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());
  }
 
  pbs_errno = 0;
  char*jobId = pbs_submit(connect, (struct attropl *)attrib,
                          scriptTmp, destination, NULL);

  if (jobId == NULL) {
    std::cerr << "******* jobId is NULL ...." << std::endl;
    std::ostringstream submit_error;     
    char* errmsg = pbs_geterrmsg(connect);

    if (errmsg != NULL)
    {
      submit_error << "TORQUE ERROR: pbs_submit: " << errmsg << std::endl;

    }
    else
    {
      submit_error << "TORQUE ERROR: pbs_submit: Error (" << pbs_errno << "-";
      submit_error << pbs_strerror(pbs_errno) << std::endl;
    }
    pbs_disconnect(connect);

    unlink(scriptTmp);

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, submit_error.str());
  }

  //pbs_disconnect(connect);
  unlink(scriptTmp);

  struct batch_status *p_status = pbs_statjob(connect, jobId, NULL, NULL);

  pbs_disconnect(connect);

  if(p_status!=NULL) {
    fillJobInfo(job, p_status);
  }
  job.setJobId(std::string(jobId));

  return 0;
}

void TorqueServer::processOptions(const TMS_Data::SubmitOptions& options, std::vector<std::string>&cmdsOptions) {

  if(options.getName().size()!=0){
    cmdsOptions.push_back("-N");
    cmdsOptions.push_back(options.getName());
  }
  if(options.getQueue().size()!=0) {
    cmdsOptions.push_back("-q");
    cmdsOptions.push_back(options.getQueue());
  }
  if(options.getOutputPath().size()!=0) {
    cmdsOptions.push_back("-o");
    cmdsOptions.push_back(options.getOutputPath());
  }
  if(options.getErrorPath().size()!=0) {
    cmdsOptions.push_back("-e");
    cmdsOptions.push_back(options.getErrorPath());
  }
  if(options.getWallTime()!=-1) {
    cmdsOptions.push_back("-l"); 
    cmdsOptions.push_back("walltime="+vishnu::convertWallTimeToString(options.getWallTime()));
  }
  if(options.getNbCpu()!=-1) {
    cmdsOptions.push_back("-l");
    std::ostringstream os_str;
    os_str << options.getNbCpu();
    cmdsOptions.push_back("ncpus="+os_str.str());
  }
  if(options.getMemory()!=-1) {
    cmdsOptions.push_back("-l");
    std::ostringstream os_str;
    os_str << options.getMemory();
    cmdsOptions.push_back("mem="+os_str.str());
  }
  if(options.getNbNodesAndCpuPerNode()!="") {
    cmdsOptions.push_back("-l");
    std::string NbNodesAndCpuPerNode = options.getNbNodesAndCpuPerNode();
    size_t posNbNodes = NbNodesAndCpuPerNode.find(":");
    if(posNbNodes!=std::string::npos) {
      std::string nbNodes = NbNodesAndCpuPerNode.substr(0, posNbNodes);
      std::string cpuPerNode = NbNodesAndCpuPerNode.substr(posNbNodes+1); 
      cmdsOptions.push_back("nodes="+nbNodes+":ppn="+cpuPerNode);
    }
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
      jobIdError << "TORQUE ERROR: pbs_deljob: illegally formed job identifier: " << tmsJobId << std::endl;
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

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());
  }

  pbs_errno = 0;
  int stat = pbs_deljob(connect, tmsJobIdOut, NULL);

  std::ostringstream pbs_del_error;
  if (stat && (pbs_errno != PBSE_UNKJOBID)) {
    char* errmsg = pbs_geterrmsg(connect);
    if(errmsg!=NULL) {
       pbs_del_error <<  "TORQUE ERROR: pbs_deljob: " << errmsg  << " " << tmsJobIdOut << std::endl;
    } else {
       pbs_del_error <<  "TORQUE ERROR: pbs_deljob: Server returned error " << pbs_errno << " for job " << tmsJobIdOut << std::endl;
    }
    pbs_disconnect(connect);
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, pbs_del_error.str());
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

int TorqueServer::getJobState(const std::string& jobId) {

  int connect;
  struct batch_status *p_status = NULL;
  struct attrl *a;
  int state = 5; //TERMINATED

  serverOut[0] = '\0'; //le bon a recuperer dans la base vishnu
  // Connect to the torque server
  connect = cnt2server(serverOut);

  if(connect <= 0) {
    return -1;
  } else {
    p_status = pbs_statjob(connect, strdup(jobId.c_str()), NULL, NULL);
    pbs_disconnect(connect);
  }

  if(p_status!=NULL) {
      a = p_status->attribs;
      while(a!=NULL) {
         if(!strcmp(a->name, ATTR_state)){
          state = convertTorqueStateToVishnuState(std::string(a->value));
          break;
         }
      a = a->next;
      }
  }

return state;
}

time_t TorqueServer::getJobProgressInfo(const std::string& jobId) {

  int connect;
  struct batch_status *p_status = NULL;
  struct attrl *a;
  time_t startTime = 0; 

  serverOut[0] = '\0'; //le bon a recuperer dans la base vishnu
  // Connect to the torque server
  connect = cnt2server(serverOut);

  if(connect <= 0) {
    return 0;
  } else {
    p_status = pbs_statjob(connect, strdup(jobId.c_str()), NULL, NULL);
    pbs_disconnect(connect);
  }

  if(p_status!=NULL) {
    a = p_status->attribs;
    while(a!=NULL) {
      if (!strcmp(a->name, ATTR_start_time)){

        std::istringstream iss(std::string(a->value));
        iss >> startTime;    
        break;
      }
      a = a->next;
    }
  }

return startTime;
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
  // If the queue is an option
  if (op.getQueue().compare("")!=0){
    fill(attr+pos, first, (char *)ATTR_q, NULL, (char *)op.getQueue().c_str(), EQ);
    pos++;
  }
}

int TorqueServer::convertTorqueStateToVishnuState(std::string state) {

  if(state.compare("T")==0) {
    return 1; //SUBMITTED
  }
  if(state.compare("Q")==0) {
    return 2; //QUEUED
  }
  if(state.compare("W")==0 || state.compare("H")==0 || state.compare("S")==0) {
    return 3; //WAITING
  }
  if(state.compare("R")==0 || state.compare("E")==0) {
    return 4; //RUNNING
  }
  if(state.compare("C")==0) {
    return 5; //TERMINATED
  } else {
    return 5;
  }
  
}

int TorqueServer::convertTorquePrioToVishnuPrio(const int& prio) {

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
      } else if(!strcmp(a->name, ATTR_qtime)){
        qtime = str;
      }
      else if (!strcmp(a->name, ATTR_etime)){ // end time ?
        etime = str;
      }	      
      a = a->next;
    }// end if name != null
  } // end while
  // TODO :
  // WORKING DIR ?
  // DESCRIPTION ?

  // Creating job
  job.setJobId(jobid);
  job.setJobName(name);
  job.setOwner(owner);
  //      job.setJobId(timeu); ? timeu ? TODO
  if (state.compare("")!=0) {
    job.setStatus(convertTorqueStateToVishnuState(state));
  } else {
    job.setStatus(0);
  }

  job.setJobQueue(location);
  job.setOutputPath(output);
  job.setErrorPath(error);

  if (prio.compare("")!=0) {
    job.setJobPrio(convertTorquePrioToVishnuPrio(convertToInt(prio.c_str())));
  } else {
    job.setJobPrio(100);
  }

  if (ncpus.compare("")!=0) {
    std::ostringstream os_ncpus;
    os_ncpus << ncpus.c_str();
    job.setNbCpus(vishnu::convertToInt(ncpus.c_str()));
  } else {
    job.setNbCpus(0);
  }

  if (qtime.compare("")!=0) {
    std::istringstream isQtime(qtime.c_str()); 
    long lQtime;
    isQtime >> lQtime;
    job.setSubmitDate(lQtime);
  } else {
    job.setSubmitDate(0);
  }

  job.setGroupName(group);

  if (etime.compare("")!=0) {
    std::istringstream isEtime;
    long lEtime;
    isEtime >> lEtime;
    job.setEndDate(lEtime);
  } else {
    job.setEndDate(0);
  }

  if (wall.compare("")!=0) {
    job.setWallClockLimit(vishnu::convertStringToWallTime(std::string(wall.c_str())));
  } else {
    job.setWallClockLimit(0);
  }

  if (mem.compare("")!=0) {
    job.setMemLimit(atoi(mem.c_str()));
  } else {
    job.setMemLimit(0);
  }

  if (node.compare("")!=0) {
    job.setNbNodes(vishnu::convertToInt(node.c_str()));
  } else {
    job.setNbNodes(0);
  }
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
  string jobid             = string("");
  string name              = string("");
  string state             = string("");
  string wall              = string("");
  string etime             = string("");
  string stime             = string("");
  string nodeAndCpu        = string("");
  string cput              = string("");
  string wallRemaining     = string("");

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
      else if (!strcmp(a->name, ATTR_qtime)){ // start time ?
        stime = str;
      } else if(!strcmp(a->name, "Walltime")){
        if(!strcmp(a->resource, "Remaining")) {
          wallRemaining = a->value;
        }
      }      
      a = a->next;
    }// end if name != null
  } // end while

  double start; 
  double end; 
  double percent = double(0);

  // Creating job
  job.setJobId(jobid);
  job.setJobName(name);
  if (state.compare("")!=0) {
    job.setStatus(convertTorqueStateToVishnuState(state));
  } else {
    job.setStatus(0);
  }

  //end = convertToInt(etime);
  start = convertToInt(stime);
  job.setEndTime(start+convertStringToWallTime(wall)); //a voir

  start = convertToInt(stime);
  job.setStartTime(start);

  if (wall.compare("")!=0) {
    job.setWallTime(convertStringToWallTime(wall));
  } else {
    job.setWallTime(0);
  }

  std::cout << "cput=" << convertToInt(cput) << std::endl;
  std::cout << "walltime =" << convertStringToWallTime(wall) << std::endl;
  std::cout << "remaining walltime = " << convertToInt(wallRemaining) << std::endl;
  //job.setPercent(converToInt(cput)/convertToInt(wall));
  double remainingTime = convertToInt(wallRemaining);
  if(remainingTime > double(0)) {
    percent = 100*(1-remainingTime/convertStringToWallTime(wall));
  }
  std::cout << "percent = " << percent << std::endl;
  job.setPercent(percent);
}

TMS_Data::ListProgression*
TorqueServer::getJobProgress(TMS_Data::ProgressOptions op){

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mprog = ecoreFactory->createListProgression();

  struct attropl attr[NBFIELDLISTJOBOPT];
  char *errmsg;
  std::string errorMsg;
  int   connect;
  struct batch_status *p_status;

  serverOut[0] = '\0'; //le bon nom a recuperer dans la base vishnu

  // Connect to the torque server
  connect = cnt2server(serverOut);

  std::cout << "connect = " << connect << std::endl;
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

  std::cout << "connect=" << connect << "op.getJobId()=" << op.getJobId() << std::endl;
  // If jobid given, the info about this job are gotten
  if(op.getJobOwner().size()!=0) {
    bool b=true; 
    fill(attr, b, (char *)ATTR_u, NULL, (char *)op.getJobOwner().c_str(), EQ);
    p_status = pbs_selstat(connect, attr, NULL);
  }
  else if (op.getJobId().size()!=0){ //TODO : combinaison d'options  a faire
    p_status = pbs_statjob(connect, (char *)op.getJobId().c_str(), NULL, NULL);
  } else{
    p_status = pbs_selstat(connect, NULL, NULL);
  }
  // If error listing
  if(p_status==NULL) {
    errmsg = pbs_geterrmsg(connect);
     if(errmsg!=NULL)
    {
      errorMsg = "TORQUE: pbs_stat: ";
      errorMsg.append(std::string(errmsg));
    }
    else {
      errorMsg = "TORQUE: pbs_stat: unknown parameters values\n";
    }

    pbs_disconnect(connect);
    throw TMSVishnuException (ERRCODE_BATCH_SCHEDULER_ERROR, errorMsg);
  }
  // Disconnect from torque
  pbs_disconnect(connect);
 
  // Local variables to get values
  int nbJobs = 0;
  struct batch_status *p;

  // For each result
  for(p = p_status; p!=NULL; p = p->next) {

    TMS_Data::Progression_ptr job = ecoreFactory->createProgression();
    // Creating the corresponding progression
    fillJobProgress(*job, p);

    nbJobs++;

    // Adding created job to list
    mprog->getProgress().push_back(job);
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


TMS_Data::ListQueues*
TorqueServer::listQueues(const std::string& OptqueueName) { 

  int connect;
  std::string errorMsg;

  serverOut[0] = '\0'; //le bon a recuperer dans la base vishnu
  // Connect to the torque server
  connect = cnt2server(serverOut);

  if (connect <= 0)
  {
    errorMsg = "TORQUE: pbs_statque: cannot connect to server ";

    if (getenv("PBSDEBUG") != NULL)
    {
      errorMsg.append("TORQUE: pbs_statque: pbs_server daemon may not be running on host");
      errorMsg.append(" or hostname in file '$TORQUEHOME/server_name' may be incorrect\n");
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errorMsg);
  }


  struct batch_status *p_status;
  if(OptqueueName.size()!=0) {
    p_status = pbs_statque(connect, strdup(OptqueueName.c_str()), NULL, NULL);
  } else {
    p_status = pbs_statque(connect, NULL, NULL, NULL);
  }

  if(p_status==NULL)
  {
    char* errmsg = pbs_geterrmsg(connect);
    if(errmsg!=NULL)
    {
      errorMsg = "TORQUE: pbs_statque: ";
      errorMsg.append(std::string(errmsg));
    }
    else {
      errorMsg = "TORQUE: pbs_statque: getting status of server\n";
    }

    pbs_disconnect(connect);
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errorMsg);
  }

  pbs_disconnect(connect);

  int nbqueue = 0;
  int nbRunningJobs = 0; 
  int nbJobsInQueue = 0;
  int tot_nbRunningJobs  = 0;
  int tot_nbJobsInQueue  = 0;
  struct batch_status *p;
  struct attrl *a;


  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistQueues = ecoreFactory->createListQueues();

  for(p = p_status; p!=NULL; p = p->next)
  {
    TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();
    queue->setName(std::string(p->name));
    queue->setWallTime(0);
    queue->setMemory(-1);
    queue->setNode(0);
    queue->setNbRunningJobs(0);
    queue->setNbJobsInQueue(0);
    queue->setMaxProcCpu(-1);
    queue->setMaxJobCpu(-1);

    a = p->attribs;
    while(a!=NULL)
    {

      std::cout << "==============a->name=" << a->name << std::endl;
      if(a->name!=NULL) {
        if(!strcmp(a->name, ATTR_start)) {
          if(*a->value == 'T') {
            queue->setState(2); //RUNNING = 'R';
          } else {
            queue->setState(1); // STARTED = 'S';
          }
        }  else if(!strcmp(a->name, ATTR_count)) { 
          //std::cout << "a->value=" << a->value << std::endl;
          // a->value=Transit:X Queued:X Held:X Waiting:W Running:X Exiting:X            
          std::string str = std::string(a->value);
          size_t found = 0;
          nbJobsInQueue = 0;
          nbRunningJobs = 0;
          //Queued
          found = str.find(':');
          str = str.substr(str.find(':', found+1)+1);
          nbJobsInQueue += vishnu::convertToInt(str.substr(0, str.find(' ')));

          //Held
          found = str.find(':');
          str = str.substr(found+1);
          nbJobsInQueue += vishnu::convertToInt(str.substr(0, str.find(' '))); 

          //Waiting
          found = str.find(':');
          str = str.substr(found+1);
          nbJobsInQueue += vishnu::convertToInt(str.substr(0, str.find(' ')));

          //Running
          found = str.find(':');
          str = str.substr(found+1);
          nbRunningJobs += vishnu::convertToInt(str.substr(0, str.find(' '))); 
          
          queue->setNbJobsInQueue(nbJobsInQueue);
          queue->setNbRunningJobs(nbRunningJobs);
        } else if(!strcmp(a->name, ATTR_rescmax)){
          if(!strcmp(a->resource, "mem")) {
            std::string walltime = std::string(a->value);

            size_t pos = walltime.find_first_not_of("0123456789");
            if(pos!=std::string::npos) {
              walltime = walltime.substr(0, pos);
            } 
            //A Verifier : si la taille de la memoire est en byte?
            queue->setMemory(vishnu::convertToInt(walltime));  
          } else if(!strcmp(a->resource, "ncpus")) {
            queue->setMaxProcCpu(vishnu::convertToInt(std::string(a->value)));
          }
          if(!strcmp(a->resource, "walltime")){
            queue->setWallTime(vishnu::convertStringToWallTime(std::string(a->value)));      
          } else if(!strcmp(a->resource, "nodect")) {
            queue->setNode(vishnu::convertToInt(std::string(a->value)));
          }
        } else if (!strcmp(a->name, ATTR_p)){ 
          queue->setPriority(convertTorquePrioToVishnuPrio(vishnu::convertToInt(std::string(a->value))));
        } else if (!strcmp(a->name, ATTR_comment)) {
          queue->setDescription(std::string(a->value));
        }

        a = a->next;
      }
    }
    // Adding created job to the list
    mlistQueues->getQueues().push_back(queue);
  }
  mlistQueues->setNbQueues(mlistQueues->getQueues().size());
  return mlistQueues;
}

TorqueServer::~TorqueServer() {
}

