/* This file is a part of VISHNU.

* Copyright SysFera SA (2011)

* contact@sysfera.com

* This software is a computer program whose purpose is to provide
* access to distributed computing resources.
*
* This software is governed by the CeCILL  license under French law and
* abiding by the rules of distribution of free software.  You can  use,
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info".

* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability.
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or
* data to be ensured and,  more generally, to use and operate it in the
* same conditions as regards security.
*
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/

/**
 * \file PbsProServer.cpp
 * \brief This file contains the VISHNU PbsProServer class.
 * \author Amine Bsila (amine.bsila@sysfera.com)
 * \date September 2012
 */


#include <vector>
#include <sstream>

#include <boost/algorithm/string.hpp>

extern "C" {
#include "pbs_ifl.h" //PbsPro includes
#include "pbs_error.h"
#include "cmds.h"
#include "pbs_sub.h"
}

#include "PbsProServer.hpp"
#include "TMSVishnuException.hpp"
#include "utilVishnu.hpp"

using namespace std;
using namespace vishnu;

/**
 * \brief Constructor
 */
PbsProServer::PbsProServer():BatchServer() {
  serverOut[0] = '\0'; //serveur par defaut
}

/**
 * \brief Function to submit PbsPro job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param job The job data structure
 * \param envp The list of environment variables used by PbsPro submission function
 * \return raises an exception on error
 */
int
PbsProServer::submit(const char* scriptPath,
                     const TMS_Data::SubmitOptions& options,
                     TMS_Data::Job& job, char** envp) {

  char destination[PBS_MAXDEST];
  char scriptTmp[MAXPATHLEN + 1] = "";
  struct attrl *attrib= NULL;
  char errMsg[1024];
  int argc = 0;

  std::vector<std::string> cmdsOptions;
  //processes the options
  processOptions(scriptPath, options, cmdsOptions);
  argc = cmdsOptions.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmdsOptions.size(); i++) {
   argv[i+2] = const_cast<char*>(cmdsOptions[i].c_str());
  }
  for(int i=0; i < argc;i++) {
    std::cout << "argv["<< i <<"] = " << argv[i] << std::endl;
  }
  std::string nbNodesStr;
  if(!options.getNbNodesAndCpuPerNode().empty()) {
    size_t posNbNodes = (options.getNbNodesAndCpuPerNode()).find(":");
    if(posNbNodes!=std::string::npos) {
      nbNodesStr = (options.getNbNodesAndCpuPerNode()).substr(0, posNbNodes);
    }
  } else {
    int nbCpu;
    int nbNodes;
    nbNodes = getPbsProNbNodesInScript(scriptPath, nbCpu);
    if(nbNodes==-1) {
      nbNodes=1;
    }
    std::ostringstream osNbnodes;
    osNbnodes << nbNodes;
    nbNodesStr = osNbnodes.str();
  }

  destination[0] = '\0';
  serverOut[0] = '\0';
  //parses the scripthPath and sets the options values
  pbs_prepare_script(argc, argv, envp, scriptTmp, destination, serverOut, &attrib);

  errMsg[0] = '\0';
  get_pbs_error_msg(errMsg);


  if(errMsg[0]!='\0') {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "PBS ERROR: "+std::string(errMsg));
  }

  int connect = cnt2server(serverOut);
  if (connect <= 0) {
    std::ostringstream connect_error;
    connect_error << "PBS ERROR: pbs_submit: cannot connect to server ";
    connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbse_to_txt(pbs_errno) << std::endl;

    if (getenv("PBSDEBUG") != NULL)
    {
      connect_error << "PBS ERROR: pbs_submit: pbs_server daemon may not be running on host";
      connect_error << "or hostname in file '$PBSHOME/server_name' may be incorrect" << std::endl;
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
      submit_error << "PBS ERROR: pbs_submit: " << errmsg  << std::endl;

    }
    else
    {
      submit_error << "PBS ERROR: pbs_submit: Error (" << pbs_errno << "-";
      submit_error << pbse_to_txt(pbs_errno) << std::endl;
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
 * \param scriptPath The job script path
 * \param options the object which contains the SubmitOptions options values
 * \param cmdsOptions The list of the option value
 * \return raises an exception on error
 */
void
PbsProServer::processOptions(const char* scriptPath,
                             const TMS_Data::SubmitOptions& options,
                             std::vector<std::string>&cmdsOptions) {

  if(!options.getNbNodesAndCpuPerNode().empty() && options.getNbCpu()!=-1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the NbCpu option and NbNodesAndCpuPerNode option together.\n");
  }

  if(options.isSelectQueueAutom() && !options.getQueue().empty() ) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the SelectQueueAutom (-Q) and getQueue (-q) options together.\n");
  }

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

    string format = getFormatedCpuPerNode(options.getNbCpu(), scriptPath);

    if(!format.empty()) {
      cmdsOptions.push_back(format);
    } else {
      cmdsOptions.push_back("nodes=1:ppn="+vishnu::convertToString(options.getNbCpu()));
    }

  }
  if(options.getMemory()!=-1) {
    cmdsOptions.push_back("-l");
    std::ostringstream os_str;
    os_str << options.getMemory();
    cmdsOptions.push_back("mem="+os_str.str()+"mb");
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
      cmdsOptions.push_back("abe");
    } else {
      throw UserException(ERRCODE_INVALID_PARAM, notification+" is an invalid notification type:"+" consult the vishnu user manuel.\n");
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

  if(options.isSelectQueueAutom()) {
    int node = 0;
    int cpu = -1;
    istringstream isNode;
    std::string optionNodesValue = options.getNbNodesAndCpuPerNode();
    if(optionNodesValue.empty()) {
      node = getPbsProNbNodesInScript(scriptPath, cpu);
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
          TMS_Data::ListQueues* resourceMin =  queuesResourceMin(queueName);

          std::string walltimeStr = getPbsProResourceValue(scriptPath, "walltime");
          long walltime = options.getWallTime()==-1?vishnu::convertStringToWallTime(walltimeStr):options.getWallTime();
          long qwalltimeMax = queue->getWallTime();
          long qwalltimeMin = ((resourceMin->getQueues()).get(0))->getWallTime();

          int qCpuMax = queue->getMaxProcCpu();
          int qCpuMin = ((resourceMin->getQueues()).get(0))->getMaxProcCpu();

          if(walltime >= qwalltimeMin && (walltime <= qwalltimeMax || qwalltimeMax==0) &&
              (cpu >= qCpuMin && cpu <= qCpuMax)){
            cmdsOptions.push_back("-q");
            cmdsOptions.push_back(queueName);
            break;
          }
          delete resourceMin;
        };
      }
    }
  }
}

/**
* \brief Function to get the script submission options
* \param scriptPath The job script path
* \param cmdsOptions The list of the option value
* \return raises an exception on error
*/
/*void
getScriptOptions(const char* scriptPath,
                     std::vector<std::string>& cmdsOptions){
  std::string scriptContent = vishnu::get_file_content(scriptPath);
  std::istringstream iss(scriptContent);
  std::string line;
  std::string value;
  std::string key;
  while(!iss.eof()) {
    getline(iss, line);
    size_t pos = line.find('#');
    if(pos==string::npos) {
      continue;
    }
    line = line.erase(0, pos);
    if(boost::algorithm::starts_with(line, "#PBS")){
      line = line.substr(std::string("#PBS").size());
      pos = line.find("-");
      if(pos!=std::string::npos){
        line = line.erase(0, pos);
        size_t pos1 = line.find_first_of("");
        if(pos1!=std::string::npos) {
          key = line.substr(0,pos1-1);
          cmdsOptions.push_back(key);
          line = line.substr(pos1);
        
          boost::algorithm::trim(line);        
          while((pos = line.find(","))!=std::string::npos){
            value = ;
          }
        }
        

        
      }
    }
  }
  
  
}*/

/**
 * \brief Function to treat the default submission options
 * \param scriptPath The job script path
 * \param options the object which contains the SubmitOptions options values
 * \param cmdsOptions The list of the option value
 * \return raises an exception on error
 */
/*void
processDefaultOptions(std::vector<std::string>& scriptOptions,
                      std::vector<std::string>& cmdsOptions){


  
}*/

/**
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int
PbsProServer::cancel(const char* jobId) {
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
PbsProServer::pbs_cancel(const char* jobId,
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
      jobIdError << "PBS ERROR: pbs_deljob: illegally formed job identifier: " << tmsJobId << std::endl;
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
    connect_error << "PBS ERROR: cannot connect to server ";
    connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbse_to_txt(pbs_errno) << std::endl;

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());
  }

  pbs_errno = 0;
  int stat = pbs_deljob(connect, tmsJobIdOut, NULL);

  std::ostringstream pbs_del_error;
  if (stat && (pbs_errno != PBSE_UNKJOBID)) {
    char* errmsg = pbs_geterrmsg(connect);
    if(errmsg!=NULL) {
       pbs_del_error <<  "PBS ERROR: pbs_deljob: " << errmsg  << " " << tmsJobIdOut << std::endl;
    } else {
       pbs_del_error <<  "PBS ERROR: pbs_deljob: Server returned error " << pbs_errno << " for job " << tmsJobIdOut << std::endl;
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
PbsProServer::getJobState(const std::string& jobId) {

  int connect;
  struct batch_status *p_status = NULL;
  struct attrl *a;
  int state = 5; //TERMINATED

  char tmsJobIdOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];

  if (get_server(strdup(jobId.c_str()), tmsJobIdOut, serverOut))
  {
    std::ostringstream jobIdError;
    jobIdError << "PBS ERROR: pbs_deljob: illegally formed job identifier: " << jobId.c_str() << std::endl;
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,  jobIdError.str());
  } else {
    serverOut[0] = '\0';
  }

  // Connect to the PbsPro server
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
           state = convertPbsProStateToVishnuState(std::string(a->value));
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
PbsProServer::getJobStartTime(const std::string& jobId) {

  int connect;
  struct batch_status *p_status = NULL;
  struct attrl *a;
  time_t startTime = 0;

  char tmsJobIdOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];

  if (get_server(strdup(jobId.c_str()), tmsJobIdOut, serverOut))
  {
    std::ostringstream jobIdError;
    jobIdError << "PBS ERROR: pbs_deljob: illegally formed job identifier: " << jobId.c_str() << std::endl;
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,  jobIdError.str());
  } else {
    serverOut[0] = '\0';
  }

  // Connect to the PbsPro server
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
      if (!strcmp(a->name, ATTR_stime)){

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
 * \brief Function to convert the PbsPro state into VISHNU state
 * \param state the state to convert
 * \return VISHNU state
 */
int
PbsProServer::convertPbsProStateToVishnuState(std::string state) {

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
 * \brief Function to convert the PbsPro priority into VISHNU priority
 * \param prio the priority to convert
 * \return VISHNU state
 */
int
PbsProServer::convertPbsProPrioToVishnuPrio(const int& prio) {

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
PbsProServer::fillJobInfo(TMS_Data::Job &job, struct batch_status *p){
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
  string qtime      = string("");
  string group      = string("");
  string mem        = string("");
  string node       = string("1");
  string wall       = string("");
  string etime      = string("");
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
      else if (!strcmp(a->name, ATTR_l)){ // resource_list

        if(!strcmp(a->resource, "mem")){
          mem = str;
        }
        else if(!strcmp(a->resource, "walltime")){
          wall = str;
        }
        else if(!strcmp(a->resource, "nodes")){ // node and nodeandcpupernode
          node = str;
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
        etime =str;
      }
      a = a->next;
    }// end if name != null
  } // end while

  // Creating job
  job.setJobId(jobid);
  job.setBatchJobId(jobid);
  job.setJobName(name);
  job.setOwner(owner);
  if (state.compare("")!=0) {
    job.setStatus(convertPbsProStateToVishnuState(state));
  } else {
    job.setStatus(0);
  }

  job.setJobQueue(location);
  job.setOutputPath(output);
  job.setErrorPath(error);

  if (prio.compare("")!=0) {
    job.setJobPrio(convertPbsProPrioToVishnuPrio(convertToInt(prio.c_str())));
  } else {
    job.setJobPrio(100);
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
    job.setMemLimit(convertPbsProMem(mem));
  } else {
    job.setMemLimit(0);
  }

  if (node.compare("")!=0) {
    int nbCpu;
    int maxNbCpu;
    int nbNodes = getNbNodesInNodeFormat(node, nbCpu, maxNbCpu);
    job.setNbNodes(nbNodes);
    job.setNbNodesAndCpuPerNode(vishnu::convertToString(nbNodes)+":"+vishnu::convertToString(nbCpu));
    job.setNbCpus(nbCpu);
  } else {
    job.setNbNodes(1);
    job.setNbNodesAndCpuPerNode("1:1");
  }

  job.setJobWorkingDir(workingDir);
}


/**
 * \brief Function to request the status of queues
 * \param optQueueName (optional) the name of the queue to request
 * \return The requested status in to ListQueues data structure
 */
TMS_Data::ListQueues*
PbsProServer::listQueues(const std::string& optqueueName) {

  int connect;
  std::string errorMsg;

  serverOut[0] = '\0'; //le bon a recuperer dans la base vishnu
  // Connect to the PbsPro server
  connect = cnt2server(serverOut);

  if (connect <= 0)
  {
    errorMsg = "PBS: pbs_statque: cannot connect to server ";

    if (getenv("PBSDEBUG") != NULL)
    {
      errorMsg.append("PBS: pbs_statque: pbs_server daemon may not be running on host");
      errorMsg.append(" or hostname in file '$PBSHOME/server_name' may be incorrect\n");
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errorMsg);
  }


  struct batch_status *p_status;
  if(optqueueName.size()!=0) {
    p_status = pbs_statque(connect, strdup(optqueueName.c_str()), NULL, NULL);
  } else {
    p_status = pbs_statque(connect, NULL, NULL, NULL);
  }

  if(p_status==NULL)
  {
    char* errmsg = pbs_geterrmsg(connect);
    if(errmsg!=NULL)
    {
      errorMsg = "PBS: pbs_statque: ";
      errorMsg.append(std::string(errmsg));
    }
    else {
      errorMsg = "PBS: pbs_statque: getting status of server\n";
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
            std::string mem = std::string(a->value);
            queue->setMemory(convertPbsProMem(mem));
          } else if(!strcmp(a->resource, "ncpus")) {
            queue->setMaxProcCpu(vishnu::convertToInt(std::string(a->value)));
          }
          if(!strcmp(a->resource, "walltime")){
            queue->setWallTime(vishnu::convertStringToWallTime(std::string(a->value)));
          } else if(!strcmp(a->resource, "nodect")) {
            int nbCpu;
            int maxNbCpu;
            int nbNodes = getNbNodesInNodeFormat(std::string(a->value), nbCpu, maxNbCpu);
            queue->setNode(nbNodes);
            queue->setMaxJobCpu(nbCpu);
          }
        } else if (!strcmp(a->name, ATTR_p)){
          queue->setPriority(convertPbsProPrioToVishnuPrio(vishnu::convertToInt(std::string(a->value))));
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
 * \brief Function to get the number of nodes in the PbsPro node format
 * \param format The node format
 * \param nbCpu The minimum number of cpu per node
 * \return the number of node
 */
int
PbsProServer::getNbNodesInNodeFormat(const std::string& format,
                                     int& nbCpu,
                                     int& maxNbCpu) {

  std::string nextNodeContent;
  std::string nextNode;
  std::string nbCpuStr;
  nbCpu = std::numeric_limits<int>::max();
  int nbNodes = 0;
  size_t beg = 0;
  char delim = '+';
  std::string ppn=":ppn=";
  size_t end = format.find(delim);
  if(end==std::string::npos){
    nextNodeContent = format.substr(beg, end-beg);
    computeNbNodesAndNbCpu(nextNodeContent, ppn,nbNodes, nbCpu, maxNbCpu);
  }

  while(end!=std::string::npos) {
    nextNodeContent = format.substr(beg, end-beg);
    computeNbNodesAndNbCpu(nextNodeContent, ppn,nbNodes, nbCpu, maxNbCpu);
    beg = end+1;
    end = format.find(delim, beg);
    //last node
    if(end==std::string::npos){
      nextNodeContent = format.substr(beg, end-beg);
      computeNbNodesAndNbCpu(nextNodeContent, ppn,nbNodes, nbCpu, maxNbCpu);
    }
  }
  if(nbNodes==0) {
    nbNodes=1;
  }
  if(nbCpu==std::numeric_limits<int>::max()){
    nbCpu = 1;
  }
  return nbNodes;
}

/**
 * \brief Function to get the PbsPro formated cpu per node
 * \param cpu The given cpu in string
 * \param scriptPath The path of the script that enventually contain the node format or the number of node
 * \return formated cpu per node
 */
std::string
PbsProServer::getFormatedCpuPerNode(const int& cpu,
                                    const std::string& scriptPath) {

  std::string nbCpuStr = vishnu::convertToString(cpu);
  std::string scriptContent = vishnu::get_file_content(scriptPath);
  std::istringstream iss(scriptContent);
  std::string line;
  std::string nodeValue;
  while(!iss.eof()) {
    getline(iss, line);
    size_t pos = line.find('#');
    if(pos==string::npos) {
      continue;
    }
    line = line.erase(0, pos);
    if(boost::algorithm::starts_with(line, "#PBS")){
      line = line.substr(std::string("#PBS").size());
      pos = line.find("-l");
      if(pos!=std::string::npos){
        pos = line.find("nodes=", pos+2);
        if(pos!=std::string::npos){
          nodeValue = line.substr(pos);

          if(!nodeValue.empty()) {
            istringstream iss(nodeValue);
            iss >> nodeValue;

            std::string ppn=":ppn=";
            size_t pos = nodeValue.find(ppn);
            while(pos!=std::string::npos) {
              std::string oldPPNValue;
              size_t posFirstChar = nodeValue.find_first_not_of("0123456789", pos+ppn.size());
              if(posFirstChar!=std::string::npos) {
                oldPPNValue = nodeValue.substr(pos+ppn.size(), posFirstChar-(pos+ppn.size()));
              } else {
                oldPPNValue =  nodeValue.substr(pos+ppn.size());
              }
              nodeValue.replace(pos+ppn.size(), oldPPNValue.size(), nbCpuStr);
              pos = nodeValue.find(ppn, pos+1);
            }

            char delim = '+';
            size_t beginPosTonken = 0;
            size_t endPosToken = nodeValue.find(delim);
            if(endPosToken==std::string::npos) {
              findAndInsert(ppn, ppn+nbCpuStr, beginPosTonken, endPosToken, nodeValue);
            }
            while(endPosToken!=std::string::npos) {
              findAndInsert(ppn, ppn+nbCpuStr, beginPosTonken, endPosToken, nodeValue);
              beginPosTonken = endPosToken+1;
              endPosToken = nodeValue.find(delim, beginPosTonken);
              //last token
              if(endPosToken==std::string::npos){
                findAndInsert(ppn, ppn+nbCpuStr, beginPosTonken, endPosToken, nodeValue);
              }
            }
          }
        }
      }
    }
  }

  return nodeValue;
}


/**
 * \brief Function to get the PbsPro number of nodes in script
 * \param scriptPath The path of the script that enventually contain the node format or the number of node
 * \param nbCpu The maximum number of cpus in the script
 * \return the number of nodes in the given script
 */
int
PbsProServer::getPbsProNbNodesInScript(const std::string& scriptPath, int& maxNbCpu) {

  std::string scriptContent = vishnu::get_file_content(scriptPath);
  std::istringstream iss(scriptContent);
  std::string line;
  int nbNodes = -1;
  while(!iss.eof()) {
    getline(iss, line);
    size_t pos = line.find('#');
    if(pos==string::npos) {
      continue;
    }
    line = line.erase(0, pos);
    if(boost::algorithm::starts_with(line, "#PBS")){
      line = line.substr(std::string("#PBS").size());
      pos = line.find("-l");
      if(pos!=std::string::npos){
        pos = line.find("nodes=");
        if(pos!=std::string::npos){
          line = line.substr(pos+std::string("nodes=").size());
          int nbCpu = -1;
          maxNbCpu = -1;
          nbNodes = getNbNodesInNodeFormat(line, nbCpu, maxNbCpu);
        }
      }
    }
  }

  return nbNodes;
}

/**
 * \brief Function to insert some additional content (valueToInsert)
 * \param valueToFind string to find
 * \param valueToInsert to insert
 * \param begin The begin position of the substring in src
 * \param end The end position of the substring in src
 * \param src The string to modify
 */
void
PbsProServer::findAndInsert(const std::string& valueToFind,
                            const std::string& valueToInsert,
                            const size_t& begin,
                            size_t& end,
                            std::string& src) {

  std::string tmp;
  if(end!=std::string::npos) {
   tmp = src.substr(begin, end-begin);
  } else {
   tmp = src.substr(begin);
  }
  if(tmp.find(valueToFind)==std::string::npos) {
    src.insert(begin+tmp.size(), valueToInsert);
    if(end!=std::string::npos) {
      end += valueToInsert.size();
    }
  }
}

/**
 * \brief Function to compute the number of nodes and cpus in the PbsPro format nodes
 * \param nextNodeContent The node content to parse
 * \param ppn The syntaxe containing the number of processors per node
 * \param nbNodes The computed number of nodes
 * \param nbCpu The numbers of cpus
 * \param nbCpu The maximum numbers of cpus
 */
void
PbsProServer::computeNbNodesAndNbCpu(const std::string& nextNodeContent,
                                     const std::string& ppn,
                                     int& nbNodes,
                                     int& nbCpu,
                                     int& maxNbCpu) {

    std::string nbCpuStr;
    size_t posColon = nextNodeContent.find(':');
    std::string nextNode = nextNodeContent.substr(0, posColon);
    size_t pos = nextNodeContent.find(ppn);
    if(pos!=std::string::npos) {
      size_t posFirstChar = nextNodeContent.find_first_not_of("0123456789", pos+ppn.size());
      if(posFirstChar!=std::string::npos) {
        nbCpuStr = nextNodeContent.substr(pos+ppn.size(), posFirstChar-(pos+ppn.size()));
      } else {
        nbCpuStr = nextNodeContent.substr(pos+ppn.size());
      }
      nbCpu = (nbCpu > vishnu::convertToInt(nbCpuStr))?vishnu::convertToInt(nbCpuStr):nbCpu;
      maxNbCpu = (maxNbCpu < vishnu::convertToInt(nbCpuStr))?vishnu::convertToInt(nbCpuStr):maxNbCpu;
    } else {
      nbCpu = 1;
      maxNbCpu = 1;
    }

    if(nextNode.find_first_not_of("0123456789")==std::string::npos) {
      nbNodes += vishnu::convertToInt(nextNode);
    } else {
      nbNodes +=1;
    }
}

/**
 * \brief Function to convert PbsPro memory into mb
 * \param memStr memory to convert
 * \return the converted memory
 */
int
PbsProServer::convertPbsProMem(const std::string& memStr) {

  size_t posSuffix = memStr.find_first_not_of("0123456789");
  int mem;

  if(posSuffix!=std::string::npos) {
    //default of vishnu mem is in megabyte
    mem = vishnu::convertToInt(memStr.substr(0, posSuffix));
    std::string suffix = memStr.substr(posSuffix);
    if(suffix.compare("b")==0) {
      mem = mem +((1 << 20)-1);
      mem = mem >> 20;
    } else if(suffix.compare("kb")==0) {
      mem = mem + ((1 << 10)-1);
      mem = mem >> 10;
    } else if(suffix.compare("gb")==0) {
      mem = mem << 10;
    } else if(suffix.compare("tb")==0) {
      mem = mem << 20;
    } else if(suffix.compare("w")==0) {
      mem = mem +((1 << 17)-1);
      mem = mem >> 17;
    } else if(suffix.compare("kw")==0) {
      mem = mem + ((1 << 7)-1);
      mem = mem >> 7;
    } else if(suffix.compare("mw")==0) {
      mem = mem << 3;
    } else if(suffix.compare("gw")==0) {
      mem = mem << 13;
    } else if(suffix.compare("tw")==0) {
      mem = mem << 23;
    }
  } else { //default value of PbsPro is in byte
    mem = vishnu::convertToInt(memStr);
    mem = mem +((1 << 20)-1);
    mem = mem >> 20;
  }
  return mem;
}

/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore
 */
void PbsProServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                                  const std::vector<string>& ignoredIds) {

   int connect = cnt2server(serverOut);

   if (connect <= 0)
   {
     std::ostringstream connect_error;

     connect_error << "PBS ERROR: pbs_selstat: cannot connect to server ";
     connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbse_to_txt(pbs_errno) << std::endl;

     if (getenv("PBSDEBUG") != NULL)
     {
       connect_error <<  "PBS ERROR: pbs_server daemon may not be running on host";
       connect_error <<  " or hostname in file '$PBSHOME/server_name' may be incorrect" << endl;
     }
     throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());
   }

   struct batch_status* p_status = pbs_selstat(connect, NULL, NULL, NULL);
   pbs_disconnect(connect);

   if(p_status!=NULL)
   {
     std::string batchId;
     int jobStatus;
     long nbRunningJobs = 0;
     long nbWaitingJobs = 0;
     struct batch_status *p;
     for(p = p_status; p!=NULL; p = p->next)
     {
       // Getting job idx
       std::string batchId = p->name;
       size_t pos_found =  batchId.find(".");
       if (pos_found!=std::string::npos) {
         pos_found=batchId.find(".", pos_found+1);
       }
       batchId=batchId.substr(0, pos_found);
       std::vector<std::string>::const_iterator iter;
       iter = std::find(ignoredIds.begin(), ignoredIds.end(), batchId);
       if(iter==ignoredIds.end()) {
         TMS_Data::Job_ptr job = new TMS_Data::Job();
         fillJobInfo(*job, p);
         jobStatus = job->getStatus();
         if(jobStatus==4) {
           nbRunningJobs++;
         } else if(jobStatus >= 1 && jobStatus <= 3) {
           nbWaitingJobs++;
         }

         listOfJobs->getJobs().push_back(job);
       }
     }
     listOfJobs->setNbJobs(listOfJobs->getJobs().size());
     listOfJobs->setNbRunningJobs(listOfJobs->getNbRunningJobs()+nbRunningJobs);
     listOfJobs->setNbWaitingJobs(listOfJobs->getNbWaitingJobs()+nbWaitingJobs);
   }
}

/**
 * \brief Function to get the value of PbsPro resource (nodes, walltime, cpu) in the script
 * \param file The file contain the script to scan
 * \param resourceName The name of the option (nodes, walltime, cpu) whose value will be returned
 * \return The value of the resource
 */
std::string
PbsProServer::getPbsProResourceValue(const char* file, const std::string& resourceName) {

  std::string resourceValue;
  std::string pbsPrefix = "#PBS";
  std::string line;
  ifstream ifile(file);
  if (ifile.is_open()) {
    while (!ifile.eof()) {
      getline(ifile, line);
      size_t pos = line.find('#');
      if(pos==string::npos) {
        continue;
      }
      line = line.erase(0, pos);
      if(boost::algorithm::starts_with(line,pbsPrefix)){
        line = line.substr(pbsPrefix.size());
        pos = line.find("-l");
        if(pos!=std::string::npos){
          pos = line.find(resourceName+"=", pos+2);
          if(pos!=std::string::npos){
            resourceValue = line.substr(pos+resourceName.size()+1);
          }
        }
      }
    }

    ifile.close();
  }
  return resourceValue;
}

/**
 * \brief Function to request the status of queues
 * \param optQueueName (optional) the name of the queue to request
 * \return The requested status in to ListQueues data structure
 */
TMS_Data::ListQueues*
PbsProServer::queuesResourceMin(const std::string& optqueueName) {

  int connect;
  std::string errorMsg;

  serverOut[0] = '\0'; //le bon a recuperer dans la base vishnu
  // Connect to the PbsPro server
  connect = cnt2server(serverOut);

  if (connect <= 0)
  {
    errorMsg = "PBS: pbs_statque: cannot connect to server ";

    if (getenv("VPBSDEBUG") != NULL)
    {
      errorMsg.append("PBS: pbs_statque: pbs_server daemon may not be running on host");
      errorMsg.append(" or hostname in file '$PBSHOME/server_name' may be incorrect\n");
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errorMsg);
  }


  struct batch_status *p_status;
  if(optqueueName.size()!=0) {
    p_status = pbs_statque(connect, strdup(optqueueName.c_str()), NULL, NULL);
  } else {
    p_status = pbs_statque(connect, NULL, NULL, NULL);
  }
  if(p_status==NULL)
  {
    char* errmsg = pbs_geterrmsg(connect);
    if(errmsg!=NULL)
    {
      errorMsg = "PBS: pbs_statque: ";
      errorMsg.append(std::string(errmsg));
    }
    else {
      errorMsg = "PBS: pbs_statque: getting status of server\n";
    }

    pbs_disconnect(connect);
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, errorMsg);
  }

  pbs_disconnect(connect);

  struct batch_status *p;
  struct attrl *a;


  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  TMS_Data::ListQueues* listQueuesResourceMin = ecoreFactory->createListQueues();

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
        if(!strcmp(a->name, ATTR_rescmin)){
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
          queue->setPriority(convertPbsProPrioToVishnuPrio(vishnu::convertToInt(std::string(a->value))));
        } else if (!strcmp(a->name, ATTR_comment)) {
          queue->setDescription(std::string(a->value));
        }

        a = a->next;
      }
    }
    // Adding created queue to the list
    listQueuesResourceMin->getQueues().push_back(queue);
  }
  listQueuesResourceMin->setNbQueues(listQueuesResourceMin->getQueues().size());
  return listQueuesResourceMin;
}

/**
 * \brief Destructor
 */
PbsProServer::~PbsProServer() {
}
