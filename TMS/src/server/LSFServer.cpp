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

#include <boost/algorithm/string.hpp>

#include <unistd.h>
#include <pwd.h>
#include <grp.h>


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

  std::vector<std::string> cmdsOptions;
  //processes the vishnu options
  processOptions(scriptPath, options, cmdsOptions);

  int argc = cmdsOptions.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[argc-1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmdsOptions.size(); i++) {
    argv[i+1] = const_cast<char*>(cmdsOptions[i].c_str());
  }

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
LSFServer::processOptions(const char* scriptPath,
                            const TMS_Data::SubmitOptions& options, 
                            std::vector<std::string>&cmdsOptions) {

  if(!options.getNbNodesAndCpuPerNode().empty() && options.getNbCpu()!=-1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the NbCpu option and NbNodesAndCpuPerNode option together.\n");
  }

  if(options.isSelectQueueAutom() && !options.getQueue().empty() ) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the SelectQueueAutom (-Q) and getQueue (-q) options together.\n");
  }

  if(options.getName().size()!=0){
    cmdsOptions.push_back("-J");
    cmdsOptions.push_back(options.getName());
  }
  if(options.getQueue().size()!=0) {
    cmdsOptions.push_back("-p");
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
    cmdsOptions.push_back("-t"); 
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
    cmdsOptions.push_back(timeStr);
  }
  if(options.getNbCpu()!=-1) {
    std::ostringstream os_str;
    os_str << options.getNbCpu();
    cmdsOptions.push_back("--mincpus="+os_str.str());
  }
  if(options.getMemory()!=-1) {
    std::ostringstream os_str;
    os_str << options.getMemory();
    cmdsOptions.push_back("--mem="+os_str.str());
  }
  if(options.getNbNodesAndCpuPerNode()!="") {
    std::string NbNodesAndCpuPerNode = options.getNbNodesAndCpuPerNode();
    size_t posNbNodes = NbNodesAndCpuPerNode.find(":");
    if(posNbNodes!=std::string::npos) {
      std::string nbNodes = NbNodesAndCpuPerNode.substr(0, posNbNodes);
      std::string cpuPerNode = NbNodesAndCpuPerNode.substr(posNbNodes+1); 
      cmdsOptions.push_back("--nodes="+nbNodes);
      cmdsOptions.push_back("--mincpus="+cpuPerNode);
    }
  }

  if(options.getMailNotification()!="") {
    std::string notification = options.getMailNotification();
    if(notification.compare("BEGIN")==0) {
      cmdsOptions.push_back("--mail-type=BEGIN");
    } else if(notification.compare("END")==0) {
      cmdsOptions.push_back("--mail-type=END");
    } else if(notification.compare("ERROR")==0) {
      cmdsOptions.push_back("--mail-type=FAIL");
    } else if(notification.compare("ALL")==0) {
      cmdsOptions.push_back("--mail-type=ALL");
    } else {
      throw UserException(ERRCODE_INVALID_PARAM, notification+" is an invalid notification type:"+" consult the vishnu user manuel");
    }
  }

  if(options.getMailNotifyUser()!="") {
    cmdsOptions.push_back("--mail-user="+options.getMailNotifyUser());
  }

  if(options.getGroup()!="") {
    cmdsOptions.push_back("--gid="+options.getGroup());
  }

  if(options.getWorkingDir()!="") {
    cmdsOptions.push_back("-D");
    cmdsOptions.push_back(options.getWorkingDir());
  }

  if(options.getCpuTime()!="") {
    cmdsOptions.push_back("-t");
    cmdsOptions.push_back(options.getCpuTime());
  }

  if(options.isSelectQueueAutom()) {
    int node = 0;
    int cpu = -1;
    istringstream isNode;
    std::string optionNodesValue = options.getNbNodesAndCpuPerNode();
    if(optionNodesValue.empty()) {
      std::string nodeStr = getLSFResourceValue(scriptPath, "-N", "--nodes");
      std::string cpuStr =  getLSFResourceValue(scriptPath, "", "--mincpus");
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

          std::string walltimeStr = getLSFResourceValue(scriptPath, "-t", "--time");
          long walltime = options.getWallTime()==-1?vishnu::convertStringToWallTime(walltimeStr):options.getWallTime();
          long qwalltimeMax = queue->getWallTime();
          int qCpuMax = queue->getMaxProcCpu();

          if((walltime <= qwalltimeMax || qwalltimeMax==0) &&
              (cpu <= qCpuMax)){
            cmdsOptions.push_back("-p");
            cmdsOptions.push_back(queueName);
            break;
          }
        };
      }
    }
  }

}

/**
 * \brief Function To convert vishnu job Id to LSF job Id 
 * \param jobId: vishnu job Id
 * \return the converted LSF job id
 */
uint32_t LSFServer::convertToLSFJobId(const std::string& jobId) {

  uint32_t LSFJobId;
  std::istringstream is(jobId);
  is >> LSFJobId;

return LSFJobId;
}

/**
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int
LSFServer::cancel(const char* jobId) {
  
  uint32_t LSFJobId = convertToLSFJobId(jobId);
  
  int res = LSF_kill_job(LSFJobId, SIGKILL, false);
  if(res) {
    char* errorMsg = LSF_strerror(res);
    if(errorMsg!=NULL) {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+std::string(errorMsg));
    } else {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: LSFServer::cancel: Unknown error");
    }
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

  uint32_t LSFJobId = convertToLSFJobId(jobId);

  int res;
  job_info_msg_t * job_buffer_ptr = NULL;
  res = LSF_load_job(&job_buffer_ptr, LSFJobId, 1);

  int state = 5; //TERMINATED 
  if(!res) {
    job_info_t LSFJobInfo = job_buffer_ptr->job_array[0];
    state = convertLSFStateToVishnuState(LSFJobInfo.job_state);
  } else {
    if(res==SLURM_PROTOCOL_SOCKET_IMPL_TIMEOUT || res==SLURM_PROTOCOL_VERSION_ERROR) {
      state = -1;
    }
  }

  if(job_buffer_ptr!=NULL) {
    LSF_free_job_info_msg(job_buffer_ptr);
  }

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

  uint32_t LSFJobId = convertToLSFJobId(jobId);

  int res;
  job_info_msg_t * job_buffer_ptr = NULL;
  res = LSF_load_job(&job_buffer_ptr, LSFJobId, 1);

  if(!res) {
    job_info_t LSFJobInfo = job_buffer_ptr->job_array[0];
    startTime = LSFJobInfo.start_time;
  }

  if(job_buffer_ptr!=NULL) {
    LSF_free_job_info_msg(job_buffer_ptr);
  }

  return startTime;
} 

/**
 * \brief Function to convert the LSF state into VISHNU state 
 * \param state the state to convert
 * \return VISHNU state 
 */
int 
LSFServer::convertLSFStateToVishnuState(const uint16_t& state) {

  int res = 0;
  switch(state) {
    case JOB_PENDING:case JOB_SUSPENDED:
      res = 3;//WAITING
      break;
    case JOB_RUNNING:
      res = 4;//RUNNING
      break;
    case JOB_COMPLETE:case JOB_FAILED:case JOB_NODE_FAIL:case JOB_TIMEOUT:
      res = 5; //TERMINATED
      break;
    case JOB_CANCELLED:
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
 * \param jobId: The identifier of the job to load
 */
void
LSFServer::fillJobInfo(TMS_Data::Job &job, const uint32_t& jobId){


}

/**
 * \brief Function to request the status of queues 
 * \param optQueueName (optional) the name of the queue to request 
 * \return The requested status in to ListQueues data structure 
 */
TMS_Data::ListQueues*
LSFServer::listQueues(const std::string& OptqueueName) { 

}

/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore 
 */
void LSFServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
    const std::vector<string>& ignoredIds) {


}

/**
 * \brief Destructor
 */
LSFServer::~LSFServer() {
}

