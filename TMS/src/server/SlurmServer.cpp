/**
 * \file SlurmServer.hpp
 * \brief This file contains the VISHNU SlurmServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <vector>
#include <sstream>

extern "C" {
#include <slurm.h> //Slurm includes
//#include "slurm_parse_script.h"
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

  int argc = cmdsOptions.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[argc-1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmdsOptions.size(); i++) {
   argv[i+1] = const_cast<char*>(cmdsOptions[i].c_str());
  }

  //parses the scripthPath and sets the options values
  //slurm_prepare_script(argc, argv, envp, scriptTmp, destination, serverOut, &attrib);
  
  return 0;
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
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int
SlurmServer::cancel(const char* jobId) {
 return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobId the identifier of the job 
 * \return -1 if the job is unknown or server not  unavailable 
 */
int 
SlurmServer::getJobState(const std::string& jobId) {

  int state = 5; //TERMINATED

return state;
}

/**
 * \brief Function to get the start time of the job
 * \param jobId the identifier of the job 
 * \return 0 if the job is unknown or server not  unavailable
 */
time_t 
SlurmServer::getJobStartTime(const std::string& jobId) {

  time_t startTime = 0; 

return startTime;
} 

/**
 * \brief Function to convert the Slurm state into VISHNU state 
 * \param state the state to convert
 * \return VISHNU state 
 */
int 
SlurmServer::convertSlurmStateToVishnuState(std::string state) {

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

/**
 * \brief Function to convert the Slurm priority into VISHNU priority
 * \param prio the priority to convert
 * \return VISHNU state 
 */
int 
SlurmServer::convertSlurmPrioToVishnuPrio(const int& prio) {

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
SlurmServer::fillJobInfo(TMS_Data::Job &job){

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

