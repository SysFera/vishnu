/**
 * \file SlurmServer.hpp
 * \brief This file contains the VISHNU SlurmServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <vector>
#include <sstream>
#include <algorithm>
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

  int pipeJobIdfd[2];
  if(pipe(pipeJobIdfd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
    close(pipeErrorfd[0]);
    close(pipeErrorfd[1]);
    close(pipeJobIdfd[0]);
    close(pipeJobIdfd[1]);
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
    std::stringstream os;
    os << jobId;
    char* jobIdStr = const_cast<char*>(strdup(os.str().c_str()));
    close(pipeJobIdfd[0]);
    write(pipeJobIdfd[1], jobIdStr, strlen(jobIdStr));
    close(pipeJobIdfd[1]);
    
    xfree(desc.script);
    slurm_free_submit_response_response_msg(resp); 
    
    exit(EXIT_SUCCESS);
  
  } else { /* Parent reads from pipe */

    close(pipeErrorfd[1]); /* Close unused write end */
    close(pipeOutfd[1]); /* Close unused write end */
    close(pipeJobIdfd[1]); /* Close unused write end */
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
    std::string jobIdStr;
    while(read(pipeJobIdfd[0], &c, 1)) {
      jobIdStr +=c;
    }
    close(pipeJobIdfd[0]);
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
    cout << "++++++++++JobIdValue = " << jobIdStr << std::endl;

    uint32_t jobId = convertToSlurmJobId(jobIdStr);
      
    fillJobInfo(job, jobId);
    
    return 0;
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
    case JOB_PENDING:JOB_SUSPENDED:
                     res = 3;//WAITING
                     break;
    case JOB_RUNNING:
                     res = 4;//RUNNING
                     break;
    case JOB_COMPLETE:case JOB_FAILED:case JOB_NODE_FAIL:case JOB_TIMEOUT:
                     res = 5; //TERMINATED
                     break;
    case JOB_CANCELLED:
                     res = 6;
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
    //job.setOutputPath(std::string(std::string(llJobInfo.step_list[0]->iwd)+"/"+(llJobInfo.step_list[0])->out)) ;
    //job.setErrorPath(std::string(std::string(llJobInfo.step_list[0]->iwd)+"/"+(llJobInfo.step_list[0])->err));
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
    job.setWallClockLimit(slurmJobInfo.time_limit);
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
 * \brief Function to request the status of queues 
 * \param optQueueName (optional) the name of the queue to request 
 * \return The requested status in to ListQueues data structure 
 */
TMS_Data::ListQueues*
SlurmServer::listQueues(const std::string& OptqueueName) { 

  std::string errorMsg;


  int nbRunningJobs = 0; 
  int nbJobsInQueue = 0;

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistQueues = ecoreFactory->createListQueues();

  {
    TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();
    // Adding created queue to the list
    mlistQueues->getQueues().push_back(queue);
  }
  mlistQueues->setNbQueues(mlistQueues->getQueues().size());
  return mlistQueues;
}

/**
 * \brief Destructor
 */
SlurmServer::~SlurmServer() {
}

