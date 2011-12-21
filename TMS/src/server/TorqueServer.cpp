/**
 * \file TorqueServer.hpp
 * \brief This file contains the VISHNU TorqueServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <vector>
#include <sstream>

extern "C" {
#include "pbs_ifl.h" //Torque includes
#include "pbs_error.h"
#include "cmds.h"
#include "pbs_sub.h"
}

#include "TorqueServer.hpp"
#include "TMSVishnuException.hpp"
#include "utilVishnu.hpp"


using namespace std;
using namespace vishnu;

/**
 * \brief Constructor
 */
TorqueServer::TorqueServer():BatchServer() {
  serverOut[0] = '\0'; //serveur par defaut
}

/**
 * \brief Function to submit Torque job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param job The job data structure
 * \param envp The list of environment variables used by Torque submission function 
 * \return raises an exception on error
 */
int 
TorqueServer::submit(const char* scriptPath, 
                     const TMS_Data::SubmitOptions& options, 
                     TMS_Data::Job& job, char** envp) {

  char destination[PBS_MAXDEST];
  char scriptTmp[MAXPATHLEN + 1] = "";
  struct attrl *attrib= NULL;
  char errMsg[1024];
  int argc = 0;

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

  for(int i=0; i < argc; i++) {
   cout << argv[i] << " ";
  }
  cout << endl;

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

  unlink(scriptTmp);
  struct batch_status *p_status = pbs_statjob(connect, jobId, NULL, NULL);

  pbs_disconnect(connect);

  if(p_status!=NULL) {
    fillJobInfo(job, p_status);
  }
  job.setJobId(std::string(jobId));

  return 0;
}

/**
 * \brief Function to treat the submission options
 * \param options the object which contains the SubmitOptions options values
 * \param cmdsOptions The list of the option value
 * \return raises an exception on error
 */
void 
TorqueServer::processOptions(const TMS_Data::SubmitOptions& options, 
                             std::vector<std::string>&cmdsOptions) {

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

  if(options.getMailNotification()!="") {
    cmdsOptions.push_back("-m");  
    std::string notification = options.getMailNotification();
    if(notification.compare("BEGIN")==0) {
      cmdsOptions.push_back("b"); 
    } else if(notification.compare("END")==0) {
      cmdsOptions.push_back("e");
    } else if(notification.compare("ERROR")==0) {
      cmdsOptions.push_back("a");
    } else if(notification.compare("ALL")==0) {
      cmdsOptions.push_back("bea");
    } else {
      throw UserException(ERRCODE_INVALID_PARAM, notification+" is an invalid notification type:"+" consult the vishnu user manuel");
    }
  }

  if(options.getMailNotifyUser()!="") {
    cmdsOptions.push_back("-M");
    cmdsOptions.push_back(options.getMailNotifyUser());
  } 

  if(options.getGroup()!="") {
    cmdsOptions.push_back("-W");
    cmdsOptions.push_back("group_list="+options.getGroup());
  }

  if(options.getWorkingDir()!="") {
    cmdsOptions.push_back("-d");
    cmdsOptions.push_back(options.getWorkingDir());
  }

  if(options.getCpuTime()!="") {
    cmdsOptions.push_back("-l");
    cmdsOptions.push_back("cput="+options.getCpuTime());
  }

}

/**
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int
TorqueServer::cancel(const char* jobId) {
  char remoteServer[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];
  return pbs_cancel(jobId, remoteServer);
}

/**
 * \brief Function to cancel job on remote machine
 * \param jobId The identifier of the job
 * \param remoteServer the name of the remote server
 * \param isLocal is the remoteServer is local
 * \return raises an exception on error
 */
int 
TorqueServer::pbs_cancel(const char* jobId, 
                         char remoteServer[], 
                         bool isLocal) {

  char tmsJobId[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];  
  char tmsJobIdOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];
  char serverOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];
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

/**
 * \brief Function to get the status of the job
 * \param jobId the identifier of the job 
 * \return -1 if the job is unknown or server not  unavailable 
 */
int 
TorqueServer::getJobState(const std::string& jobId) {

  int connect;
  struct batch_status *p_status = NULL;
  struct attrl *a;
  int state = 5; //TERMINATED

  char tmsJobIdOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];

  if (get_server(strdup(jobId.c_str()), tmsJobIdOut, serverOut))
  {
    std::ostringstream jobIdError;
    jobIdError << "TORQUE ERROR: pbs_deljob: illegally formed job identifier: " << jobId.c_str() << std::endl;
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,  jobIdError.str());
  } else {
    serverOut[0] = '\0';
  }

  // Connect to the torque server
  connect = cnt2server(serverOut);

  if(connect <= 0) {
    return -1;
  } else {
    p_status = pbs_statjob(connect, tmsJobIdOut, NULL, NULL);
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

/**
 * \brief Function to get the start time of the job
 * \param jobId the identifier of the job 
 * \return 0 if the job is unknown or server not  unavailable
 */
time_t 
TorqueServer::getJobStartTime(const std::string& jobId) {

  int connect;
  struct batch_status *p_status = NULL;
  struct attrl *a;
  time_t startTime = 0; 

  char tmsJobIdOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];

  if (get_server(strdup(jobId.c_str()), tmsJobIdOut, serverOut))
  {
    std::ostringstream jobIdError;
    jobIdError << "TORQUE ERROR: pbs_deljob: illegally formed job identifier: " << jobId.c_str() << std::endl;
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,  jobIdError.str());
  } else {
    serverOut[0] = '\0';
  }

  // Connect to the torque server
  connect = cnt2server(serverOut);

  if(connect <= 0) {
    return 0;
  } else {
    p_status = pbs_statjob(connect, tmsJobIdOut, NULL, NULL);
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

/**
 * \brief Function to convert the Torque state into VISHNU state 
 * \param state the state to convert
 * \return VISHNU state 
 */
int 
TorqueServer::convertTorqueStateToVishnuState(std::string state) {

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
    
  return 5; //TERMINATED
  
}

/**
 * \brief Function to convert the Torque priority into VISHNU priority
 * \param prio the priority to convert
 * \return VISHNU state 
 */
int 
TorqueServer::convertTorquePrioToVishnuPrio(const int& prio) {

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

/**
 * \brief Function To fill the info concerning a job
 * \fn void fillJobInfo(TMS_Data::Job_ptr job, struct batch_status *p)
 * \param job: The job to fill
 * \param p: The batch status structure containing the job info
 */
void
TorqueServer::fillJobInfo(TMS_Data::Job &job, struct batch_status *p){
  struct attrl *a;
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
  string workingDir = string(""); 
  // Getting job idx
  str = p->name;
  pos_found =  str.find(".");
  if (pos_found!=std::string::npos) {
    pos_found =  str.find(".", pos_found+1);
  }
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
            tmp = str.substr(pos_found); 
            pos_found = tmp.find("=");
            if(pos_found!=string::npos) {
              nodeAndCpu = node+":"+tmp.substr(pos_found+1);
            } else {
              nodeAndCpu = str;
            }
          }
          else{
            node = str;
          }
        }
      } else if(!strcmp(a->name, ATTR_v)) { // working_dir
        std::string env = "PBS_O_WORKDIR";
        string value = a->value;
        size_t pos = value.find(env);
        if(pos!=std::string::npos) {
          pos = value.find("=",pos+env.size());
          size_t end = value.find(",",pos+1);
          if(end!=std::string::npos) {
            workingDir= value.substr(pos+1, end-pos-1);
          }
        } else {
          workingDir= value.substr(pos+1);
        }
      } else if (!strcmp(a->name, ATTR_g)){ // group
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

  // Creating job
  job.setJobId(jobid);
  job.setJobName(name);
  job.setOwner(owner);
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
    job.setWallClockLimit(vishnu::convertStringToWallTime(wall));
  } else {
    job.setWallClockLimit(0);
  }

  if (mem.compare("")!=0) {
    job.setMemLimit(convertToInt(mem));
  } else {
    job.setMemLimit(0);
  }

  if (node.compare("")!=0) {
    job.setNbNodes(vishnu::convertToInt(node));
  } else {
    job.setNbNodes(0);
  }
  if(nodeAndCpu.size()!=0) {
    job.setNbNodesAndCpuPerNode(nodeAndCpu);
  }

  job.setJobWorkingDir(workingDir);
}


/**
 * \brief Function to request the status of queues 
 * \param optQueueName (optional) the name of the queue to request 
 * \return The requested status in to ListQueues data structure 
 */
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

  int nbRunningJobs = 0; 
  int nbJobsInQueue = 0;
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

      if(a->name!=NULL) {
        if(!strcmp(a->name, ATTR_start)) {
          if(*a->value == 'T') {
            queue->setState(2); // RUNNING
          } else {
            queue->setState(1); // STARTED
          }
        }  else if(!strcmp(a->name, ATTR_count)) { 
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
    // Adding created queue to the list
    mlistQueues->getQueues().push_back(queue);
  }
  mlistQueues->setNbQueues(mlistQueues->getQueues().size());
  return mlistQueues;
}

/**
 * \brief Destructor
 */
TorqueServer::~TorqueServer() {
}

