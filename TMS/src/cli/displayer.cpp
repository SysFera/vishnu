
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_time.hpp"
#include "utilVishnu.hpp"
#include "displayer.hpp"
#include "constants.hpp"
#include "tmsUtils.hpp"
#include <boost/format.hpp>

using namespace std;
using namespace vishnu;
using namespace TMS_Data;

/**
 * \brief To display the output of all job
 * \param j: the list of jobs output to display
 */
void
displayAllJobOutput(TMS_Data::ListJobResults& listJobs) {
  cout << setw(75) << setfill('-') << ""
       << endl << setw(10)  << left << setfill(' ')<< " Job ID" << " |  Location of Downloaded Files"
       << endl << setw(75) << setfill('-') << "" << setfill(' ') << endl ;

  for (size_t i = 0 ; i < listJobs.getResults().size() ; i++) {
    JobResult* job = listJobs.getResults().get(i);
    cout << setw(10) << left << (" " +job->getJobId()) ;
    cout << " | " <<job->getOutputDir();
    printJobResultSummary(job);
  }
  cout << "\nNumber of downloaded jobs : " << listJobs.getNbJobs() << endl;
}

/**
 * \brief To display the output of a job
 * \param j: the job output result
 */
void
displayJobOutput(TMS_Data::JobResult_ptr j){
  cout << " ----------------------------------------------------------------\n";
  cout << setw(10) << left << " Job ID" << " |  Location of Downloaded Files\n";
  cout << " ----------------------------------------------------------------\n";
  cout << setw(10) << left << (" " + j->getJobId()) << " | " << j->getOutputDir();
  printJobResultSummary(j);

}

/**
 * \brief To display the job info
 * \param j: The job to display the info
 */
void
displayJob(TMS_Data::Job& job){


  size_t pos = job.getOutputPath().find(":");
  std::string prefixOutputPath = (pos == std::string::npos)? job.getMachine()+":" : "";
  pos = job.getErrorPath().find(":");
  std::string prefixErrorPath = (pos == std::string::npos)? job.getMachine()+":" : "";
  boost::posix_time::ptime pt;

  cout << " Job                    : " << job.getId()  << " (Batch ID : "<<job.getBatchJobId() <<")";
  cout << "\n Work                 : " << (job.getWorkId()!=-1? convertToString(job.getWorkId()) : "-");
  if (! job.getRelatedSteps().empty()) {
    cout << "\n Related Jobs         : " << job.getRelatedSteps();
  }
  cout << "\n User                 : " << job.getUserId();
  cout << "\n Session              : " << job.getSessionId();
  cout << "\n Machine              : " << job.getMachine() << " (Host: " << job.getMachine() << ")";
  cout << "\n Output path (remote) : " << prefixOutputPath+job.getOutputPath();
  cout << "\n Error path  (remote) : " << prefixErrorPath+job.getErrorPath();
  cout << "\n Output dir (remote)  : " << job.getOutputDir();
  cout << "\n Job                  : " << job.getId()  << " (Batch/Process ID: " << job.getBatchJobId() <<")";
  if (! job.getVmId().empty()) {
    cout << "\n Virtual Machine      : " << job.getVmId() << " (IP: " << job.getVmIp()<< ")";
  }
  cout << "\n Work                 : " << (job.getWorkId()!=-1? convertToString(job.getWorkId()) : "-");
  cout << "\n Job name             : " << job.getName();
  cout << "\n Job path             : " << job.getPath();
  cout << "\n CPU per Node         : " << job.getNbCpus();
  cout << "\n Working dir (remote) : " << job.getWorkingDir();
  cout << "\n Status               : " << vishnu::convertJobStateToString(job.getStatus());
  if (job.getStatus() == vishnu::STATE_FAILED) {
    cout << "\n Submit error       : " << job.getSubmitError();
  }
  if (job.getSubmitDate() > 0) {
    pt =  boost::posix_time::from_time_t(job.getSubmitDate());
    cout << "\n Submit date          : " << boost::posix_time::to_simple_string(pt);
  } else  {
    cout << "\n Submit date          : -";
  }

  if(job.getEndDate() > 0) {
    pt =  boost::posix_time::from_time_t(job.getEndDate());
    cout << "\n End date             : " << boost::posix_time::to_simple_string(pt);
  } else {
    cout << "\n End date             : -";
  }
  cout << "\n Owner                : " << job.getLocalAccount();
  cout << "\n Queue                : " << job.getQueue();
  cout << "\n Wall clock limit     : " << convertWallTimeToString(job.getWallClockLimit());
  cout << "\n Group name           : " << job.getGroupName();
  cout << "\n Description          : " << job.getDescription();
  if(job.getMemLimit() > 0) {
    cout << "\n Max memory           : " << job.getMemLimit();
  } else {
    cout << "\n Max memory           : -";
  }
  cout << "\n Nodes                : " << job.getNbNodes();
  cout << "\n NbNodesAndCpuPerNode : " << job.getNbNodesAndCpuPerNode();
  cout << endl;
}

/**
 * \brief To display the job info in a list
 * \param j: The progression to display the info
 */
void
displayJobProgress(ListProgression& j){
  for (size_t i = 0 ; i < j.getProgress().size() ; i++){
    displayProgress(*(j.getProgress().get(i)));
  }
}

/**
 * \brief To display the job info
 * \param p: The progression to display the info
 */
void
displayProgress(Progression& p){

  boost::posix_time::ptime pt;

  cout << " ------------------------ " << endl;
  cout << " Job Id    : " << p.getJobId() << endl;
  cout << " Job name  : " << p.getJobName() << endl;
  cout << " Wall time : " << convertWallTimeToString(p.getWallTime()) << endl;
  if(p.getStartTime() > 0) {
    pt =  boost::posix_time::from_time_t(p.getStartTime());
    cout << "Start time : " << boost::posix_time::to_simple_string(pt) << endl;
  } else {
    cout << "Start time : -" << endl;
  }
  if(p.getEndTime() > 0) {
    pt =  boost::posix_time::from_time_t(p.getEndTime());
    cout << " End time  : " << boost::posix_time::to_simple_string(pt) << endl;
  } else {
    cout << " End time  : -" << endl;
  }
  cout << " Percent   : " << p.getPercent() << "%" << endl;
  cout << " Status    : " << vishnu::convertJobStateToString(p.getStatus())<< endl;
  cout << endl;
}

/**
 * \brief To display the list of jobs
 * \param j: The list of job to display
 */
void
displayListJobs(ListJobs& j){
  for (size_t i = 0 ; i < j.getJobs().size() ; i++){
    displayJob(*(j.getJobs().get(i)));
  }
}

/**
 * \brief To display the queues info
 * \param j: The list of queues to display the info
 */
void
displayQueues(ListQueues& j){
  for (size_t i = 0 ; i < j.getQueues().size() ; i++){
    displayQueue(*(j.getQueues().get(i)));
  }
};

/**
 * \brief To display the info about a queue
 * \param q: The list of queues to display the info
 */
void
displayQueue(Queue& q){

  cout << " ------------------------ " << endl;
  cout << " Name        : " << q.getName() << endl;
  if(q.getMaxJobCpu() > 0) {
    cout << "Max job cpu  : " << q.getMaxJobCpu() << endl;
  } else {
    cout << "Max job cpu  : - " << endl;
  }
  if(q.getMaxProcCpu() > 0) {
    cout << "Max proc cpu : " << q.getMaxProcCpu() << endl;
  } else {
    cout << "Max proc cpu : - " << endl;
  }
  if(q.getMemory() > 0) {
    cout << " Memory      : " << q.getMemory() << endl;
  } else {
    cout << " Memory      : - " << endl;
  }
  if(q.getWallTime() > 0) {
    cout << " Wall time   : " << convertWallTimeToString(q.getWallTime()) << endl;
  } else {
    cout << " Wall time   : - " << endl;
  }
  if(q.getNode() > 0) {
    cout << " Node        : " << q.getNode() << endl;
  } else {
    cout << " Node        : -  " << endl;
  }
  cout << "Running jobs : " << q.getNbRunningJobs() << endl;
  cout << "Job in queue : " << q.getNbJobsInQueue() << endl;
  int state = q.getState();
  std::string stateStr = (state==1?"STARTED":(state==2?"RUNNING":"NOT_STARTED"));
  cout << "State        : " << stateStr << endl;
  cout << "Priority     : " << q.getPriority() << "(" << convertJobPriorityToString(q.getPriority()) << ")" << endl;
  cout << "Description  : " << q.getDescription() << endl;
}

/**
 * \brief To display some basic job info after submit
 * \param job: The job to submit data
 */
void
displaySubmit(TMS_Data::Job job){
  cout << "Job Id     : " << job.getId() << endl;
}


/**
 * \brief  function to convert job priority into string
 * \param state: The state of job
 * \return The converted state value
 */
std::string convertJobPriorityToString(const int& prio) {

  string prioStr;
  switch(prio) {
    case -1:
      prioStr = "UNDEFINED";
      break;
    case 0:
      prioStr = "VERY_LOW";
      break;
    case 1:
      prioStr = "VERY_LOW";
      break;
    case 2:
      prioStr = "LOW";
      break;
    case 3:
      prioStr = "NORMAL";
      break;
    case 4:
      prioStr = "HIGH";
      break;
    case 5:
      prioStr = "VERY_HIGH";
      break;
    default:
      prioStr = "UNDEFINED";
      break;
  }
  return prioStr;

}


/**
 * \brief Display a '-' caracter
 * \param size: The number of '-' to diplay
 * \The output stream in which the display will be done.
 */
void
setFill(int size, ostream& os) {

  for(int i=0; i < size; i++) {
    os << "-";
  }
  os << "  ";
}

/**
 * \brief Helper function to display a list of queues
 * \param os: The output stream in which the list will be printed
 * \param lsQueues: The list to display
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, ListQueues& lsQueues) {

  std::string name;
  int Memory;
  long walltime;
  int node;
  int nbRunJobs;
  int nbJobsQue;
  std::string stateStr;
  int state;

  std::string nameHead = "NAME";
  std::string memoryHead = "MEMORY";
  std::string walltimeHead = "WALLTIME";
  std::string nodeHead = "NODE";
  std::string nbRunJobsHead = "RUNNING JOBS";
  std::string nbJobsQueHead = "JOB IN QUEUE";
  std::string stateHead = "STATE";

  size_t maxNameSize = nameHead.size();
  size_t maxMemorySize = memoryHead.size();
  size_t maxWalltimeSize = walltimeHead.size();
  size_t maxNodeSize = nodeHead.size();
  size_t maxNbRunJobsSize = nbRunJobsHead.size();
  size_t maxNbJobsQueSize = nbJobsQueHead.size();
  size_t maxStateSize = stateHead.size();

  for(size_t i = 0; i < lsQueues.getQueues().size(); i++) {

    name = (lsQueues.getQueues().get(i))->getName();
    maxNameSize = std::max(maxNameSize, name.size());

    Memory = (lsQueues.getQueues().get(i))->getMemory();
    maxMemorySize = std::max(maxMemorySize, vishnu::convertToString(Memory).size());

    walltime = (lsQueues.getQueues().get(i))->getWallTime();
    maxWalltimeSize = std::max(maxWalltimeSize, vishnu::convertWallTimeToString(walltime).size());

    node = (lsQueues.getQueues().get(i))->getNode();
    maxNodeSize = std::max(maxNodeSize, vishnu::convertToString(node).size());

    nbRunJobs = (lsQueues.getQueues().get(i))->getNbRunningJobs();
    maxNbRunJobsSize = std::max(maxNbRunJobsSize, vishnu::convertToString(nbRunJobs).size());

    nbJobsQue = (lsQueues.getQueues().get(i))->getNbJobsInQueue();
    maxNbJobsQueSize = std::max(maxNbJobsQueSize, vishnu::convertToString(nbJobsQue).size());

    state = (lsQueues.getQueues().get(i))->getState();
    std::string stateStr = (state==1?"STARTED":(state==2?"RUNNING":"NOT_STARTED"));
    maxStateSize = std::max(maxStateSize, stateStr.size());
  }

  os << setw(maxNameSize+2) << left << nameHead << setw(maxMemorySize+2) << left << memoryHead << setw(maxWalltimeSize+2) ;
  os << left << walltimeHead << setw(maxNodeSize+2) << left << nodeHead << setw(maxNbRunJobsSize+2) << left << nbRunJobsHead;
  os << setw(maxNbJobsQueSize+2) << left << nbJobsQueHead << setw(maxStateSize+2) << left << stateHead << endl;

  setFill(maxNameSize, os);
  setFill(maxMemorySize, os);
  setFill(maxWalltimeSize, os);
  setFill(maxNodeSize, os);
  setFill(maxNbRunJobsSize, os);
  setFill(maxNbJobsQueSize, os);
  setFill(maxStateSize, os);
  os << endl;

  for(size_t i = 0; i < lsQueues.getQueues().size(); i++) {
    name = (lsQueues.getQueues().get(i))->getName();
    Memory = (lsQueues.getQueues().get(i))->getMemory();
    walltime = (lsQueues.getQueues().get(i))->getWallTime();
    node = (lsQueues.getQueues().get(i))->getNode();
    nbRunJobs = (lsQueues.getQueues().get(i))->getNbRunningJobs();
    nbJobsQue = (lsQueues.getQueues().get(i))->getNbJobsInQueue();
    state = (lsQueues.getQueues().get(i))->getState();

    os << setw(maxNameSize+2) << left << name;
    if(Memory > 0) {
      os << setw(maxMemorySize+2) << left << Memory;
    } else {
      os << setw(maxMemorySize+2) << left <<  " --- ";
    }
    if(walltime > 0) {
      os << setw(maxWalltimeSize+2) << left << vishnu::convertWallTimeToString(walltime);
    } else {
      os << setw(maxWalltimeSize+2) << left << " --- ";
    }

    if(node > 0) {
      os << setw(maxNodeSize+2) << left << node;
    } else {
      os << setw(maxNodeSize+2) << " --- ";
    }
    os << setw(maxNbRunJobsSize+2) << left << nbRunJobs;
    os << setw(maxNbJobsQueSize+2) << left << nbJobsQue;
    stateStr = (state==1?"STARTED":(state==2?"RUNNING":"NOT_STARTED"));
    os << setw(maxStateSize+2) << left << stateStr;
    os << endl;

  }

  os << endl;
  os << "The number of queues is: " << lsQueues.getNbQueues() << std::endl;
  return os;
}

/**
 * \brief Helper function to display a list of jobs
 * \param os: The output stream in which the list will be printed
 * \param listJobs: The list to display
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, ListJobs& listJobs) {

  std::string jobId;
  std::string jobName;
  std::string owner;
  std::string queue;
  int priority;
  int status;
  std::string machineId;
  int workId;
  std::string workId_;

  std::string jobIdHead = "ID";
  std::string jobNameHead = "NAME";
  std::string workIdHead = "WORK";
  std::string ownerHead = "OWNER";
  std::string statusHead = "STATUS";
  std::string queueHead = "QUEUE";
  std::string machineIdHead = "MACHINE";

  size_t maxJobIdSize = jobIdHead.size();
  size_t maxJobNameSize = jobNameHead.size();
  size_t maxOwnerSize = ownerHead.size();
  size_t maxWorkIdSize = workIdHead.size();
  size_t maxStatusSize = statusHead.size();
  size_t maxQueueSize = queueHead.size();
  size_t maxMachineIdSize = machineIdHead.size();

  for(size_t i = 0; i < listJobs.getJobs().size(); i++) {

    jobId = (listJobs.getJobs().get(i))->getId();
    maxJobIdSize = std::max(maxJobIdSize, jobId.size());

    jobName = (listJobs.getJobs().get(i))->getName();
    maxJobNameSize = std::max(maxJobNameSize, jobName.size());

    workId = (listJobs.getJobs().get(i))->getWorkId();
    workId_ = (workId!=-1? convertToString(workId) : " -- ") ;
    maxWorkIdSize = std::max(maxWorkIdSize, workId_.size());

    owner = (listJobs.getJobs().get(i))->getUserId();
    maxOwnerSize = std::max(maxOwnerSize, owner.size());

    queue = (listJobs.getJobs().get(i))->getQueue();
    maxQueueSize = std::max(maxQueueSize, queue.size());

    status = (listJobs.getJobs().get(i))->getStatus();
    maxStatusSize = std::max(maxStatusSize, vishnu::convertJobStateToString(status).size());

    machineId = (listJobs.getJobs().get(i))->getMachine();
    maxMachineIdSize = std::max(maxMachineIdSize, machineId.size());
  }

  os << setw(maxJobIdSize+2) << left << jobIdHead << setw(maxJobNameSize+2) << left << jobNameHead
     << setw(maxWorkIdSize+2) << left << workIdHead << setw(maxOwnerSize+2) << left << ownerHead
     << setw(maxStatusSize+2) << statusHead << setw(maxQueueSize+2) << left << queueHead
     << setw(maxMachineIdSize+2) << left << machineIdHead << endl;


  setFill(maxJobIdSize, os);
  setFill(maxJobNameSize, os);
  setFill(maxWorkIdSize, os);
  setFill(maxOwnerSize, os);
  setFill(maxStatusSize, os);
  setFill(maxQueueSize, os);
  setFill(maxMachineIdSize, os);
  os << endl;

  for(size_t i = 0; i < listJobs.getJobs().size(); i++) {

    jobId = (listJobs.getJobs().get(i))->getId();
    jobName = (listJobs.getJobs().get(i))->getName();
    workId = (listJobs.getJobs().get(i))->getWorkId();
    workId_ = (workId!=-1? convertToString(workId) : " -- ") ;
    owner = (listJobs.getJobs().get(i))->getUserId();
    status = (listJobs.getJobs().get(i))->getStatus();
    queue = (listJobs.getJobs().get(i))->getQueue();
    machineId = (listJobs.getJobs().get(i))->getMachine();

    os << setw(maxJobIdSize+2) << left << jobId;
    os << setw(maxJobNameSize+2) << left << jobName;
    os << setw(maxWorkIdSize+2) << left << workId_;
    os << setw(maxOwnerSize+2) << left << owner;
    os << setw(maxStatusSize+2) << left << vishnu::convertJobStateToString(status);
    os << setw(maxQueueSize+2) << left << queue;
    ostringstream oss;
    oss << priority  << "(" << convertJobPriorityToString(priority) << ")";
    os << setw(maxMachineIdSize+2) << left << machineId;
    os << endl;

  }

  os << endl;
  os << listJobs.getNbJobs() << " jobs, " << listJobs.getNbRunningJobs() << " running, ";
  os << listJobs.getNbWaitingJobs() << " waiting" << std::endl;
  return os;
}

/**
 * \brief Helper function to display a list of jobs progression
 * \param os: The output stream in which the list will be printed
 * \param listProgress: The list to display
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, ListProgression& listProgress) {


  std::string jobId;
  std::string jobName;
  long walltime;
  long startTime;
  long endTime;
  int status;
  int percent;
  boost::posix_time::ptime pt;

  std::string jobIdHead = "JOB ID";
  std::string jobNameHead = "JOB NAME";
  std::string walltimeHead = "WALLTIME";
  std::string startTimeHead = "START TIME";
  std::string endTimeHead = "END TIME";
  std::string statusHead = "STATUS";
  std::string percentHead = "PERCENT";

  size_t maxJobIdSize = jobIdHead.size();
  size_t maxJobNameSize = jobNameHead.size();
  size_t maxWalltimeSize = walltimeHead.size();
  size_t maxStartTimeSize = startTimeHead.size();
  size_t maxEndTimeSize = endTimeHead.size();
  size_t maxStatusSize = statusHead.size();
  size_t maxPercentSize = percentHead.size();

  for(size_t i = 0; i < listProgress.getProgress().size(); i++) {

    jobId = (listProgress.getProgress().get(i))->getJobId();
    maxJobIdSize = std::max(maxJobIdSize, jobId.size());

    jobName = (listProgress.getProgress().get(i))->getJobName();
    maxJobNameSize = std::max(maxJobNameSize, jobName.size());

    walltime = (listProgress.getProgress().get(i))->getWallTime();
    maxWalltimeSize = std::max(maxWalltimeSize, vishnu::convertWallTimeToString(walltime).size());

    startTime = (listProgress.getProgress().get(i))->getStartTime();
    if(startTime > 0) {
      pt =  boost::posix_time::from_time_t(startTime);
      maxStartTimeSize = std::max(maxStartTimeSize, boost::posix_time::to_simple_string(pt).size());
    }

    endTime = (listProgress.getProgress().get(i))->getEndTime();
    if(endTime > 0) {
      pt =  boost::posix_time::from_time_t(endTime);
      maxEndTimeSize = std::max(maxEndTimeSize, boost::posix_time::to_simple_string(pt).size());
    }

    status = (listProgress.getProgress().get(i))->getStatus();
    maxStatusSize = std::max(maxStatusSize, vishnu::convertJobStateToString(status).size());

    percent = (listProgress.getProgress().get(i))->getPercent();
    maxPercentSize = std::max(maxPercentSize, convertToString(percent).size());

  }

  os << setw(maxJobIdSize+2) << left << jobIdHead << setw(maxJobNameSize+2) << left << jobNameHead << setw(maxWalltimeSize+2) ;
  os << left << walltimeHead << setw(maxStartTimeSize+2) << startTimeHead  << setw(maxEndTimeSize+2) << left << endTimeHead;
  os << setw(maxStatusSize+2) << left << statusHead  << setw(maxPercentSize+2) << percentHead << endl;

  setFill(maxJobIdSize, os);
  setFill(maxJobNameSize, os);
  setFill(maxWalltimeSize, os);
  setFill(maxStartTimeSize, os);
  setFill(maxEndTimeSize, os);
  setFill(maxStatusSize, os);
  setFill(maxPercentSize, os);
  os << endl;

  for(size_t i = 0; i < listProgress.getProgress().size(); i++) {

    jobId = (listProgress.getProgress().get(i))->getJobId();
    jobName = (listProgress.getProgress().get(i))->getJobName();
    walltime = (listProgress.getProgress().get(i))->getWallTime();
    startTime = (listProgress.getProgress().get(i))->getStartTime();
    endTime = (listProgress.getProgress().get(i))->getEndTime();
    status =  (listProgress.getProgress().get(i))->getStatus();
    percent = (listProgress.getProgress().get(i))->getPercent();

    os << setw(maxJobIdSize+2) << left << jobId;
    os << setw(maxJobNameSize+2) << left << jobName;
    os << setw(maxWalltimeSize+2) << left << vishnu::convertWallTimeToString(walltime);
    if(startTime > 0) {
      pt =  boost::posix_time::from_time_t(startTime);
      os << setw(maxStartTimeSize+2) << left << boost::posix_time::to_simple_string(pt);
    } else {
      os << setw(maxStartTimeSize+2) << left << " --- ";
    }
    if(endTime > 0) {
      pt =  boost::posix_time::from_time_t(endTime);
      os << setw(maxEndTimeSize+2) << left << boost::posix_time::to_simple_string(pt);
    } else {
      os << setw(maxEndTimeSize+2) << left <<  " --- ";
    }
    os << setw(maxStatusSize+2) << left <<  vishnu::convertJobStateToString(status);
    ostringstream oss;
    oss <<  percent << "%";
    os << setw(maxPercentSize+2) << left << oss.str() ;
    os << endl;

  }

  os << endl;
  os << "The number of jobs in queue is: " << listProgress.getNbJobs() << endl;

  return os;
}


/**
 * @brief printJobResultSummary
 * @param job
 */
void
printJobResultSummary(const TMS_Data::JobResult_ptr job)
{
  std::string errorFile = (boost::format("%1%/ERROR_%2%") % job->getOutputDir() % job->getJobId()).str();
  std::string missingFile = (boost::format("%1%/MISSINGFILES_%2%") % job->getOutputDir() % job->getJobId()).str();

  cout << (boost::filesystem::exists(errorFile) ?
             (boost::format("\n\tFAILURE: yes "
                            "\n\tFAILURE DETAILS: %1%") % vishnu::get_file_content(errorFile)).str()
           : "\n\tERROR: no");
  cout << (boost::filesystem::exists(missingFile)?
             ((boost::format("\n\tMISSING FILES: %1%") % vishnu::get_file_content(missingFile))).str()
           : "\n\tMISSING FILES: no");
  cout << endl;
}
