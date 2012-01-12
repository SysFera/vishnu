/**
 * \file SlurmServer.hpp
 * \brief This file contains the VISHNU SlurmServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include <unistd.h>
#include <pwd.h>
#include <grp.h>


extern "C" {
#include <slurm/slurm.h> //Slurm includes
#include "slurm_parser.h"
}

#include "SlurmServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"

using namespace std;
using namespace vishnu;

/**
 * \brief Constructor
 */
SlurmServer::SlurmServer():BatchServer() {
  msymbolMap["\%j"] = "";
  msymbolMap["\%J"] = "";
}

/**
 * \brief Function to submit Slurm job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param job The job data structure
 * \param envp The list of environment variables used by Slurm submission function 
 * \return raises an exception on error
 */
int 
SlurmServer::submit(const char* scriptPath, 
    const TMS_Data::SubmitOptions& options, 
    TMS_Data::Job& job, char** envp) {

  std::vector<std::string> cmdsOptions;
  //processes the vishnu options
  processOptions(options, cmdsOptions);

  int argc = cmdsOptions.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[argc-1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmdsOptions.size(); i++) {
    argv[i+1] = const_cast<char*>(cmdsOptions[i].c_str());
  }

  job_desc_msg_t desc; //The slurm job descriptor
  //parse the scripthPath and sets the options values
  slurm_parse_script(argc, argv, &desc);

  std::string errorMsg;
  //Check the job output path 
  errorMsg = checkSLURMOutPutPath(desc.std_out);
  if(errorMsg.size()!=0) {
    xfree(desc.script);
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
  }
  //Check the job error path
  errorMsg = checkSLURMOutPutPath(desc.std_err, "job error path");
  if(errorMsg.size()!=0) {
    xfree(desc.script);
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
  }

  uint32_t jobId = 0;
  std::string jobOutputPath ;
  std::string jobErrorPath;
  submit_response_msg_t *resp;
  int retries = 0;
  int VISHNU_MAX_RETRIES = 5;
  //To submit the job
  while(slurm_submit_batch_job(&desc, &(resp)) < 0) {

    if (errno == ESLURM_ERROR_ON_DESC_TO_RECORD_COPY) {
      errorMsg = "Slurm job queue full, sleeping and retrying.";
    } else if (errno == ESLURM_NODES_BUSY) {
      errorMsg = "Job step creation temporarily disabled, retrying";
    } else if (errno == EAGAIN) {
      errorMsg = "Slurm temporarily unable to accept job, sleeping and retrying.";
    } else {
      errorMsg ="";
    }
    if ((errorMsg == "") || (retries >= VISHNU_MAX_RETRIES)) {
      errorMsg = "Batch job submission failed: "+std::string(slurm_strerror(errno));
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+errorMsg);
    }

    if (retries || errno == ESLURM_NODES_BUSY) {
      std::cout << errorMsg << std::endl;
    } else {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+errorMsg);
    }
    sleep (++retries);
  }

  jobOutputPath ;
  jobErrorPath;
  if(desc.std_out!=NULL) {
    jobOutputPath = desc.std_out;
  }
  if(desc.std_err!=NULL) {
    jobErrorPath = desc.std_err;
  }

  jobId = resp->job_id;
  //Fill the vishnu job structure 
  fillJobInfo(job, jobId);

  if(jobOutputPath.size()!=0) {
    replaceSymbolInToJobPath(jobOutputPath);
    job.setOutputPath(jobOutputPath);
  }
  if(jobErrorPath.size()!=0) {
    replaceSymbolInToJobPath(jobErrorPath);
    job.setErrorPath(jobErrorPath);
  }

  xfree(desc.script);
  slurm_free_submit_response_response_msg(resp);

  return 0;
}

/**
 * \brief Function to check if slurm path syntax is correct
 * \param path The path to check
 * \param pathInfo The information on path to print
 * \return an error message
 */
std::string SlurmServer::checkSLURMOutPutPath(char*& path, const std::string& pathInfo) {

  string errorMsg;
  if(path!=NULL) {
    //get the slurm treated symbols
    std::map<std::string, std::string>::const_iterator iter;
    std::map<std::string, std::string>::const_iterator end=msymbolMap.end();
    std::string vishnuTreatedSymbols;

    std::string symbol;
    if(containsAnExcludedSlurmSymbols(path, symbol)){
      ostringstream osStr;
      osStr << "VISHNU can't treats in your " << pathInfo << " the following sumbol: " << symbol << std::endl;

      for(iter=msymbolMap.begin(); iter!=end; ++iter) {
        if(iter!=msymbolMap.begin()) {
          vishnuTreatedSymbols+=" or ";
        }
        vishnuTreatedSymbols+=iter->first[0]+symbol.substr(1,symbol.size()-2)+iter->first[1];
      }
      osStr << "*****The only SLURM symbols treated by VISHNU are: " << vishnuTreatedSymbols << std::endl;
      osStr << "*****Replace your symbols by the following sumbols: "<< vishnuTreatedSymbols << std::endl;
      errorMsg = osStr.str();
    }
  }
  return errorMsg;
}

/**
 * \brief Function to replace slurm job identifer symbol by its real value in to a path
 * \param path The path containing the job symbol
 */
void SlurmServer::replaceSymbolInToJobPath(std::string& path) {

  static const int SLURM_MAX_WIDTH = 10;//This value may be change for later version of SLURM
  //actual version of slurm is 2.2.1

  std::string widthStr;
  int width;
  std::ostringstream os;

  std::map<std::string, std::string>::const_iterator iter;
  std::map<std::string, std::string>::const_iterator end=msymbolMap.end();

  for(iter=msymbolMap.begin(); iter!=end; ++iter) {
    //find the symbol position 
    size_t pos0 = path.find((iter->first)[0]);
    size_t pos1 = path.find((iter->first)[1], pos0);
    while(pos0!=std::string::npos && pos1!=std::string::npos) {
      widthStr = path.substr(pos0+1, pos1-pos0-1);
      if(widthStr.size()==0) {
        path.erase(pos0, 2);//remove symbol[0]+symbol[1]
        path.insert(pos0, iter->second);
      } else if(widthStr.find_first_not_of("0123456789")==std::string::npos) {
        width = vishnu::convertToInt(widthStr);  
        if(width > SLURM_MAX_WIDTH) {
          width = SLURM_MAX_WIDTH;
        }
        path.erase(pos0, widthStr.size()+2);//remove symbol[0]+width+symbol[1]
        os << setfill('0') << setw(width) << iter->second;
        path.insert(pos0, os.str()); 
      } else {
        if((widthStr.substr(0,1)).find_first_not_of("0123456789")==std::string::npos){
          path.erase(pos0, 1);//remove symbol[0]
        }
      }
      //Pass to the next symbol
      pos0 = path.find(iter->first[0], pos0+1);
      pos1 = path.find(iter->first[1],pos0);
      os.str("");
    }
  }
}

/**
 * \brief Function to cheick if a path contains an excluded slurm symbol by vishnu
 * \param path The path to check
 * \param symbol The excluded symbol
 * \return true if the path contain an exluded symbol
 */
bool SlurmServer::containsAnExcludedSlurmSymbols(const std::string& path, std::string& symbol) {

  std::vector<std::string> excludedSymbols;
  excludedSymbols.push_back("\%t");
  excludedSymbols.push_back("\%n");
  excludedSymbols.push_back("\%N");
  excludedSymbols.push_back("\%s");
  

  std::vector<std::string>::const_iterator iter;
  std::vector<std::string>::const_iterator end=excludedSymbols.end();
  bool ret = false;
  std::string widthStr;
  for(iter=excludedSymbols.begin(); iter!=end; ++iter) {
    //find the symbol position 
    size_t pos0 = path.find((*iter)[0]);
    size_t pos1 = path.find((*iter)[1], pos0);
    while(pos0!=std::string::npos && pos1!=std::string::npos) {
      widthStr = path.substr(pos0+1, pos1-pos0-1);
      if(widthStr.size()==0) {
        ret = true;
        symbol = *iter;
        break;
      } else if(widthStr.find_first_not_of("0123456789")==std::string::npos) {
        ret = true;
        symbol = (*iter)[0]+widthStr+(*iter)[1];
        break;
      } else {
        if((widthStr.substr(0,1)).find_first_not_of("0123456789")==std::string::npos){
          static_cast<std::string>(path).erase(pos0, 1);//remove symbol[0]
          ret = false;
        }
      }
      //Pass to the next symbol
      pos0 = path.find((*iter)[0], pos0+1);
      pos1 = path.find((*iter)[1],pos0);
    }
  }
  return ret;
}

/**
 * \brief Function to treat the submission options
 * \param options the object which contains the SubmitOptions options values
 * \param cmdsOptions The list of the option value
 * \return raises an exception on error
 */
void 
SlurmServer::processOptions(const TMS_Data::SubmitOptions& options, 
                             std::vector<std::string>&cmdsOptions) {

  if(!options.getNbNodesAndCpuPerNode().empty() && options.getNbCpu()!=-1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the NbCpu option and NbNodesAndCpuPerNode option together.\n");
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


}

/**
 * \brief Function To convert vishnu job Id to slurm job Id 
 * \param jobId: vishnu job Id
 * \return the converted slurm job id
 */
uint32_t SlurmServer::convertToSlurmJobId(const std::string& jobId) {

  uint32_t slurmJobId;
  std::istringstream is(jobId);
  is >> slurmJobId;

return slurmJobId;
}

/**
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int
SlurmServer::cancel(const char* jobId) {
  
  uint32_t slurmJobId = convertToSlurmJobId(jobId);
  
  int res = slurm_kill_job(slurmJobId, SIGKILL, false);
  if(res) {
    char* errorMsg = slurm_strerror(res);
    if(errorMsg!=NULL) {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+std::string(errorMsg));
    } else {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: SlurmServer::cancel: Unknown error");
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
SlurmServer::getJobState(const std::string& jobId) {

  uint32_t slurmJobId = convertToSlurmJobId(jobId);

  int res;
  job_info_msg_t * job_buffer_ptr = NULL;
  res = slurm_load_job(&job_buffer_ptr, slurmJobId, 1);

  int state = 5; //TERMINATED 
  if(!res) {
    job_info_t slurmJobInfo = job_buffer_ptr->job_array[0];
    state = convertSlurmStateToVishnuState(slurmJobInfo.job_state);
  } else {
    if(res==SLURM_PROTOCOL_SOCKET_IMPL_TIMEOUT || res==SLURM_PROTOCOL_VERSION_ERROR) {
      state = -1;
    }
  }

  if(job_buffer_ptr!=NULL) {
    slurm_free_job_info_msg(job_buffer_ptr);
  }

  return state;
}

/**
 * \brief Function to get the start time of the job
 * \param jobId the identifier of the job 
 * \return 0 if the job is unknown
 */
time_t 
SlurmServer::getJobStartTime(const std::string& jobId) {

  time_t startTime = 0; 

  uint32_t slurmJobId = convertToSlurmJobId(jobId);

  int res;
  job_info_msg_t * job_buffer_ptr = NULL;
  res = slurm_load_job(&job_buffer_ptr, slurmJobId, 1);

  if(!res) {
    job_info_t slurmJobInfo = job_buffer_ptr->job_array[0];
    startTime = slurmJobInfo.start_time;
  }

  if(job_buffer_ptr!=NULL) {
    slurm_free_job_info_msg(job_buffer_ptr);
  }

  return startTime;
} 

/**
 * \brief Function to convert the Slurm state into VISHNU state 
 * \param state the state to convert
 * \return VISHNU state 
 */
int 
SlurmServer::convertSlurmStateToVishnuState(const uint16_t& state) {

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
 * \brief Function to convert the Slurm priority into VISHNU priority
 * \param prio the priority to convert
 * \return VISHNU state 
 */
int 
SlurmServer::convertSlurmPrioToVishnuPrio(const uint32_t& prio) {
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
SlurmServer::fillJobInfo(TMS_Data::Job &job, const uint32_t& jobId){

  int res;
  job_info_msg_t * job_buffer_ptr = NULL;
  res = slurm_load_job(&job_buffer_ptr, jobId, 1);

  if(!res) {
   
    job_info_t slurmJobInfo = job_buffer_ptr->job_array[0];
    job.setJobId(vishnu::convertToString(jobId));
    job.setOutputPath(std::string(slurmJobInfo.work_dir)+"/slurm-"+convertToString(jobId)+".out");//default path
    job.setErrorPath(std::string(slurmJobInfo.work_dir)+"/slurm-"+convertToString(jobId)+".out");//default path
    job.setStatus(convertSlurmStateToVishnuState(slurmJobInfo.job_state));
    if(slurmJobInfo.name!=NULL) {
      job.setJobName(slurmJobInfo.name);
    }
    job.setSubmitDate(slurmJobInfo.submit_time);
    struct passwd* user = getpwuid(slurmJobInfo.user_id);
    if(user!=NULL) {
      job.setOwner(user->pw_name);
    }
    struct group* grp = getgrgid(slurmJobInfo.group_id);
    if(grp!=NULL) {
      job.setGroupName(grp->gr_name);
    }
    if(slurmJobInfo.partition!=NULL) {
      job.setJobQueue(slurmJobInfo.partition);
    }
    //Here we multiplie the time_limit by 60 because SLURM time_limit is in minutes
    if(slurmJobInfo.time_limit < ((std::numeric_limits<uint32_t>::max())/60)) {
      job.setWallClockLimit(60*(slurmJobInfo.time_limit));
    }
    job.setEndDate(slurmJobInfo.end_time);
    if(slurmJobInfo.comment!=NULL) {
      job.setJobDescription(slurmJobInfo.comment);
    }
    job.setJobPrio(convertSlurmPrioToVishnuPrio(slurmJobInfo.priority));
    job.setMemLimit(slurmJobInfo.pn_min_memory);
    uint32_t nbNodes = slurmJobInfo.num_nodes;
    uint32_t nbCpus =  slurmJobInfo.pn_min_cpus;
    job.setNbCpus(nbCpus);
    job.setNbNodes(nbNodes);
    job.setNbNodesAndCpuPerNode(convertToString(nbNodes)+":"+convertToString(nbCpus));

    //To fill the job working dir
    job.setJobWorkingDir(slurmJobInfo.work_dir);

    //fill the msymbol map
    msymbolMap["\%j"] = vishnu::convertToString(jobId);
    msymbolMap["\%J"] = vishnu::convertToString(jobId);
  } else {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: SlurmServer::fillJobInfo: slurm_load_jobs error");
  }

  if(job_buffer_ptr!=NULL) {
    slurm_free_job_info_msg(job_buffer_ptr);
  }

}

/**
 * \brief Function to compute the number of running and waiting jobs of each queue 
 * \param run contains the number of running jobs of each queue 
 * \param que contains the number of waiting jobs of each queue
 * \return non zero if error
 */
int
SlurmServer::computeNbRunJobsAndQueueJobs(std::map<std::string, size_t>& run,
                                       std::map<std::string, size_t>& que) {

  job_info_msg_t * allJobs = NULL;
  int res = slurm_load_jobs((time_t)NULL, &allJobs, 1);

  if(!res) {
    job_info_t *job = NULL;
    job = allJobs->job_array;
    for (uint32_t i = 0; i < allJobs->record_count; i++) {
      switch(job[i].job_state) {
        case JOB_PENDING: case JOB_SUSPENDED:
          que[job[i].partition]++;
          break;
        case JOB_RUNNING:
          run[job[i].partition]++;
          break;
        default:
         break;
      }
    } 
  } 

 return res;
}

/**
 * \brief Function to request the status of queues 
 * \param optQueueName (optional) the name of the queue to request 
 * \return The requested status in to ListQueues data structure 
 */
TMS_Data::ListQueues*
SlurmServer::listQueues(const std::string& OptqueueName) { 

  partition_info_msg_t *allPartition = NULL;
  partition_info_t *partition = NULL;
  int res;

  res = slurm_load_partitions((time_t)NULL, &allPartition, 1);
  if (res) {
    char* errorMsg = slurm_strerror(res);
    if(errorMsg!=NULL) {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+std::string(errorMsg));
    } else {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: SlurmServer::listQueues: Unknown error");
    }
  }

  std::map<std::string, size_t> run;
  std::map<std::string, size_t> que;
  computeNbRunJobsAndQueueJobs(run, que);

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistQueues = ecoreFactory->createListQueues();
  
  partition = allPartition->partition_array;
  bool OptQueueNameFound = false;
  for (uint32_t i = 0; i < allPartition->record_count; i++) {

    if(OptqueueName.size()!=0) {
      for (uint32_t j = 0; j < allPartition->record_count; j++) {
        if(OptqueueName.compare(std::string(partition[j].name))==0) {
          OptQueueNameFound = true;  
          i = j;
          break;
        }
      }
      if(!OptQueueNameFound){
        throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "Unknown queue name"+OptqueueName);
      }
    }

    TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();
    //Set the queue state
    switch(partition[i].state_up) { 
      case PARTITION_DOWN: 
        queue->setState(0);
        break;
      case PARTITION_INACTIVE:
        queue->setState(1);
        break;
      case PARTITION_UP:case PARTITION_DRAIN:
        queue->setState(2);
        break;
      default:
        queue->setState(0);
        break;
    }

    if (run.count(partition[i].name)) {
      queue->setNbRunningJobs(run[partition[i].name]);
    }
    if (que.count(partition[i].name)) {
      queue->setNbJobsInQueue(que[partition[i].name]);
    }

    queue->setName(partition[i].name); 
    queue->setPriority(convertSlurmPrioToVishnuPrio(partition[i].priority));

    //Here we multiplie the max_time by 60 because SLURM max_time in minutes
    if(partition[i].max_time < ((std::numeric_limits<uint32_t>::max())/60)) {
        queue->setWallTime(60*partition[i].max_time);
    }
    queue->setMemory(-1);//UNDEFINED in SLURM
    queue->setDescription("");//UNDEFINED in SLURM    
    queue->setMaxProcCpu(partition[i].total_cpus);
    queue->setMaxJobCpu(partition[i].total_cpus);
    queue->setNode(partition[i].total_nodes);

    // Adding created queue to the list
    mlistQueues->getQueues().push_back(queue);

    if(OptQueueNameFound) {
      i = allPartition->record_count;
    }
  }

  mlistQueues->setNbQueues(mlistQueues->getQueues().size());

  if(allPartition!=NULL) {
    slurm_free_partition_info_msg(allPartition);
  }

  return mlistQueues;
}

/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore 
 */
void SlurmServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
    const std::vector<string>& ignoredIds) {

  job_info_msg_t * job_buffer_ptr = NULL;
  int res = slurm_load_jobs((time_t) NULL, &job_buffer_ptr, SHOW_ALL);
  if(!res) {
    uint32_t batchId;
    int jobStatus;
    long nbRunningJobs = 0;
    long nbWaitingJobs = 0;
    for(uint32_t i=0; i < job_buffer_ptr->record_count; i++) {
      batchId = (job_buffer_ptr->job_array[i]).job_id;
      std::vector<std::string>::const_iterator iter;
      iter = std::find(ignoredIds.begin(), ignoredIds.end(), convertToString(batchId));
      if(iter==ignoredIds.end()) {
        TMS_Data::Job_ptr job = new TMS_Data::Job();
        fillJobInfo(*job, batchId);
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
 * \brief Destructor
 */
SlurmServer::~SlurmServer() {
}

