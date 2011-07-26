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
#include <slurm.h> //Slurm includes
#include "slurm-parser.h"
}

#include "SlurmServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "utilVishnu.hpp"

using namespace std;
using namespace vishnu;

/**
 * \brief Constructor
 */
SlurmServer::SlurmServer():BatchServer() {
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
  //processes the options
  processOptions(options, cmdsOptions);

  std::cout << "*******cmdsOptions content**** " << std::endl;
  std::copy(cmdsOptions.begin(), cmdsOptions.end(), ostream_iterator<std::string>(cout, " "));
  std::cout << std::endl;
  int argc = cmdsOptions.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[argc-1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmdsOptions.size(); i++) {
    argv[i+1] = const_cast<char*>(cmdsOptions[i].c_str());
  }

  std::cout << "*******argv content**** " << std::endl;
  std::copy(argv, argv+argc, ostream_iterator<char*>(cout, " "));
  std::cout << std::endl;

  int pipeErrorfd[2];
  if(pipe(pipeErrorfd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int pipeOutfd[2];
  if(pipe(pipeOutfd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
    close(pipeErrorfd[0]);
    close(pipeErrorfd[1]);
  }

  int pipeJobInfofd[2];
  if(pipe(pipeJobInfofd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
    close(pipeErrorfd[0]);
    close(pipeErrorfd[1]);
    close(pipeJobInfofd[0]);
    close(pipeJobInfofd[1]);
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if(pid==0) { /* Child writes to pipe */
    close(pipeErrorfd[0]); /* Close unused read end */
    close(pipeOutfd[0]); /* Close unused read end */

    dup2(pipeErrorfd[1], 2);
    dup2(pipeOutfd[1], 1);

    close(pipeErrorfd[1]);
    close(pipeOutfd[1]);
    job_desc_msg_t desc;
    //parses the scripthPath and sets the options values
    slurm_parse_script(argc, argv, &desc);

    submit_response_msg_t *resp;
    //To submit the job
    if(slurm_submit_batch_job(&desc, &(resp)) < 0) {
      std::cerr << "Under construction............" << std::endl;
    }

    
    uint32_t jobId = resp->job_id;
    std::string jobOutputPath ;
    std::string jobErrorPath;
    std::stringstream os;
    if(desc.std_out!=NULL) {
      jobOutputPath = desc.std_out;
    }
    if(desc.std_err!=NULL) {
      jobErrorPath = desc.std_err;
    }
    os << jobId << " " << jobOutputPath << " " << jobErrorPath;
    char* jobInfoStr = const_cast<char*>(strdup(os.str().c_str()));
    close(pipeJobInfofd[0]);
    write(pipeJobInfofd[1], jobInfoStr, strlen(jobInfoStr));
    close(pipeJobInfofd[1]);
    
    xfree(desc.script);
    slurm_free_submit_response_response_msg(resp); 
    
    exit(EXIT_SUCCESS);
  
  } else { /* Parent reads from pipe */

    close(pipeErrorfd[1]); /* Close unused write end */
    close(pipeOutfd[1]); /* Close unused write end */
    close(pipeJobInfofd[1]); /* Close unused write end */
    char c;

    std::string output;
    //To get the output message
    while(read(pipeOutfd[0], &c, 1)){
      output +=c;
    }
    close(pipeOutfd[0]);

    //To get the stderr message
    std::string errorMsg;
    while(read(pipeErrorfd[0], &c, 1)){
      errorMsg +=c;
    }
    close(pipeErrorfd[0]);

    //To get the jobId
    std::string jobInfoStr;
    while(read(pipeJobInfofd[0], &c, 1)) {
      jobInfoStr +=c;
    }
    close(pipeJobInfofd[0]);
    wait(NULL);                /* Wait for child */
    
    if(output.size()!=0) {
      std::cout <<  output << std::endl;
    }

    //To raise an exception if error message size is not empty
    if(errorMsg.size()!=0) {
      size_t size = errorMsg.size();
      string toFind("error:");
      size_t pos = errorMsg.find(toFind);
      if(pos!=string::npos){
        errorMsg = errorMsg.substr(pos+toFind.size()+1, size-pos-1);
      }

      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+std::string(errorMsg));
      std::cout << "TEST*****: " << errorMsg << std::endl;
    }
    
    std::istringstream is(jobInfoStr);
    cout << "++++++++++jobInfoStr = " << jobInfoStr << std::endl;
    std::vector<std::string> JobInfoTokens;
    std::copy(istream_iterator<std::string>(is), istream_iterator<std::string>(), back_inserter(JobInfoTokens));
    
    std::vector<std::string>::const_iterator iter = JobInfoTokens.begin();
    std::vector<std::string>::const_iterator end = JobInfoTokens.end();
    std::string jobIdStr;
    std::string jobOutputPath;
    std::string jobErrorPath;
    if(iter!=end) {
      jobIdStr = *iter;
      ++iter;
    }
    if(iter!=end) {
      jobOutputPath = *iter;
      ++iter;
    }
    if(iter!=end) {
      jobErrorPath = *iter;
      ++iter;
    }
    cout << "++++++++++JobIdValue    = " << jobIdStr << std::endl;
    cout << "++++++++++Before JobOutputPath = " << jobOutputPath << std::endl;
    cout << "++++++++++Before jobErrorPath  = " << jobErrorPath << std::endl;

    uint32_t jobId = convertToSlurmJobId(jobIdStr);
    //To fill the vishnu job structure 
    fillJobInfo(job, jobId);
   
    if(jobOutputPath.size()!=0) {
      replaceSymbolInToJobPath(jobOutputPath, "\%J", jobIdStr);
      job.setOutputPath(jobOutputPath);
    }
    if(jobErrorPath.size()!=0) {
      replaceSymbolInToJobPath(jobErrorPath, "\%J", jobIdStr);
      job.setErrorPath(jobErrorPath);
    }

    cout << "++++++++++After JobOutputPath = " << job.getOutputPath() << std::endl;
    cout << "++++++++++After jobErrorPath  = " << job.getErrorPath() << std::endl;
    return 0;
  }
  
}

/**
 * \brief Function to replace slurm job identifer symbol by its real value in to a path
 * \param path The path containing the job symbol
 * \param symbol The symbol to replace
 * \param value The value to attribute at the symbol
 */
void SlurmServer::replaceSymbolInToJobPath(std::string& path, 
    const std::string& symbol, 
    const std::string& value) {

  //find the symbol position
  size_t pos = path.find(symbol);
  while(pos!=std::string::npos) {
    //remove the symbol identifier
    path.erase(pos, symbol.size());
    //insert the correspondant value of the symbol
    path.insert(pos, value);
    //Pass to the next symbol
    pos = path.find(symbol);
  }

  //Find the symbol in to lower case
  std::string symbolInToLowerCase(symbol);
  //convert the symbol in to lower case
  std::transform(symbol.begin(), symbol.end(), symbolInToLowerCase.begin(), ::tolower);
  pos = path.find(symbolInToLowerCase);
  while(pos!=std::string::npos) {
    path.erase(pos, symbolInToLowerCase.size());
    path.insert(pos, value);
    pos = path.find(symbolInToLowerCase);
  }
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
    cmdsOptions.push_back(vishnu::convertWallTimeToString(options.getWallTime()));
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
      cmdsOptions.push_back("--nodes="+nbNodes+" --mincpus="+cpuPerNode);
    }
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
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: Unknown error");
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
    state = -1;
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
    if(slurmJobInfo.time_limit < std::numeric_limits<uint32_t>::max()) {
      job.setWallClockLimit(60*(slurmJobInfo.time_limit));
    }
    job.setEndDate(slurmJobInfo.end_time);
    if(slurmJobInfo.comment!=NULL) {
      job.setJobDescription(slurmJobInfo.comment);
    }
    job.setJobPrio(convertSlurmPrioToVishnuPrio(slurmJobInfo.priority));
    job.setMemLimit(slurmJobInfo.pn_min_memory);
    job.setNbCpus(slurmJobInfo.num_cpus);
    job.setNbNodes(slurmJobInfo.num_nodes);
  } else {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: slurm_load_jobs error");
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
SlurmServer::computeNbRunJobsAndQueueJobs(std::map<std::string, int>& run,
                                       std::map<std::string, int>& que) {

  job_info_msg_t * allJobs = NULL;
  int res = slurm_load_jobs((time_t)NULL, &allJobs, 1);

  if(!res) {
    job_info_t *job = NULL;
    job = allJobs->job_array;
    for (int i = 0; i < allJobs->record_count; i++) {
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

  int nbRunningJobs = 0; 
  int nbJobsInQueue = 0;

  res = slurm_load_partitions((time_t)NULL, &allPartition, 1);
  if (res) {
    char* errorMsg = slurm_strerror(res);
    if(errorMsg!=NULL) {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+std::string(errorMsg));
    } else {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: Unknown error");
    }
  }

  std::map<std::string, int> run;
  std::map<std::string, int> que;
  computeNbRunJobsAndQueueJobs(run, que);

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistQueues = ecoreFactory->createListQueues();
  
  partition = allPartition->partition_array;
  bool OptQueueNameFound = false;
  for (int i = 0; i < allPartition->record_count; i++) {

    if(OptqueueName.size()!=0) {
      int j = 0;
      for (int j = 0; j < allPartition->record_count; j++) {
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
    if(partition[i].max_time < std::numeric_limits<uint32_t>::max()) {
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

  slurm_free_partition_info_msg(allPartition);
  
  return mlistQueues;
}

/**
 * \brief Destructor
 */
SlurmServer::~SlurmServer() {
}

