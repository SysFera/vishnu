
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_time.hpp"
#include "utilVishnu.hpp"
#include "displayer.hpp"

using namespace std;
using namespace vishnu;

/**
 * \brief To display the output of all job
 * \param j: the list of jobs output to display
 */
void 
displayAllJobOutput(TMS_Data::ListJobResults& j){
  for (size_t i = 0 ; i < j.getResults().size() ; i++){
    displayJobOutput(j.getResults().get(i));
  }
  cout << endl;
  cout << "The number of completed jobs output is: " << j.getNbJobs() << endl;
  cout << endl;
}

/**
 * \brief To display the output of a job
 * \param j: the job output result
 */
void 
displayJobOutput(TMS_Data::JobResult_ptr j){
  cout << " ------------------------ " << endl;
  cout << " Job Id     : " << j->getJobId() << endl;
  cout << " Output path: " << j->getOutputPath() << endl;
  cout << " Error  path: " << j->getErrorPath() << endl;
  cout << endl;
}

/**
 * \brief To display the job info
 * \fn void displayJob(Job& j)
 * \param j: The job to display the info
 */
void 
displayJob(TMS_Data::Job& j){

  boost::posix_time::ptime pt;

  cout << " ------------------------ " << endl;
  cout << " Session Id           : " << j.getSessionId() << endl;
  cout << " Machine Id           : " << j.getSubmitMachineId() << endl;
  cout << " Machine name         : " << j.getSubmitMachineName() << endl;
  cout << " Job Id               : " << j.getJobId() << endl;
  cout << " Job name             : " << j.getJobName() << endl;
  cout << " Job path             : " << j.getJobPath() << endl;
  cout << " Output path (remote) : " << j.getOutputPath() << endl;
  cout << " Error path  (remote) : " << j.getErrorPath() << endl;
  cout << " Priority             : " << j.getJobPrio() << "(" << convertJobPriorityToString(j.getJobPrio()) << ")" << endl;
  cout << " CPU                  : " << j.getNbCpus() << endl;
  cout << " Working dir          : " << j.getJobWorkingDir() << endl;
  cout << " Status               : " << convertJobStateToString(j.getStatus()) << endl;
  if(j.getSubmitDate() > 0) {
    pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(j.getSubmitDate()));
    cout << " Submit date          : " << boost::posix_time::to_simple_string(pt) << endl;
  } else  {
    cout << " Submit date          : UNDEFINED" << endl;
  }

  if(j.getEndDate() > 0) {
    pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(j.getEndDate()));
    cout << " End date             : " << boost::posix_time::to_simple_string(pt) << endl;
  } else {
    cout << " End date             : UNDEFINED" << endl;
  }
  cout << " Owner                : " << j.getOwner() << endl;
  cout << " Queue                : " << j.getJobQueue() << endl;
  cout << " Wall clock limit     : " << convertWallTimeToString(j.getWallClockLimit()) << endl;
  cout << " Group name           : " << j.getGroupName() << endl;
  cout << " Description          : " << j.getJobDescription() << endl;
  if(j.getMemLimit() > 0) {
    cout << " Max memory           : " << j.getMemLimit() << endl;
  } else {
    cout << " Max memory           : UNDEFINED" << endl;
  }
  cout << " Nodes                : " << j.getNbNodes() << endl;
  cout << " CPU/Node             : " << j.getNbNodesAndCpuPerNode() << endl;
  cout << endl;
}

/**
 * \brief To display the job info in a list
 * \fn void displayJobProgress(ListProgression& j)
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
 * \fn void displayJobProgress(Progression& p)
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
    pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(p.getStartTime()));
    cout << "Start time : " << boost::posix_time::to_simple_string(pt) << endl;
  } else {
    cout << "Start time : UNDEFINED" << endl;
  }
  if(p.getEndTime() > 0) {
    pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(p.getEndTime()));
    cout << " End time  : " << boost::posix_time::to_simple_string(pt) << endl;
  } else {
    cout << " End time  : UNDEFINED" << endl;
  }
  cout << " Percent   : " << p.getPercent() << "%" << endl;
  cout << " Status    : " << convertJobStateToString(p.getStatus()) << endl;
  cout << endl;
}

/**
 * \brief To display the list of jobs
 * \fn void displayListJobs(ListJobs& j)
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
 * \fn void displayQueues(ListQueues& j)
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
 * \fn void displayQueues(ListQueues& j)
 * \param j: The list of queues to display the info
 */
void
displayQueue(Queue& q){

  cout << " ------------------------ " << endl;
  cout << " Name        : " << q.getName() << endl;
  if(q.getMaxJobCpu() > 0) {
  cout << "Max job cpu  : " << q.getMaxJobCpu() << endl;
  } else {
  cout << "Max job cpu  : UNDEFINED " << endl;
  }
  if(q.getMaxProcCpu() > 0) {
  cout << "Max proc cpu : " << q.getMaxProcCpu() << endl;
  } else {
  cout << "Max proc cpu : UNDEFINED " << endl;
  }
  if(q.getMemory() > 0) {
  cout << " Memory      : " << q.getMemory() << endl;
  } else {
  cout << " Memory      : UNDEFINED " << endl;
  }
  if(q.getWallTime() > 0) {
  cout << " Wall time   : " << convertWallTimeToString(q.getWallTime()) << endl;
  } else {
  cout << " Wall time   : UNDEFINED " << endl;
  }
  if(q.getNode() > 0) {
  cout << " Node        : " << q.getNode() << endl;
  } else {
  cout << " Node        : UNDEFINED  " << endl;
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
 * \fn void displaySubmit(TMS_Data::Job job)
 * \param job: The job to submit data
 */
void
displaySubmit(TMS_Data::Job job){
  cout << "Job Id     : " << job.getJobId() << endl;
}

/**
 * \brief  function to convert job status into string 
 * \param state: The state of job
 * \return The converted state value
 */
std::string convertJobStateToString(const int& state) {

  string stateStr;
  switch(state) {
    case 0:
      stateStr = "UNDEFINED";
      break;
    case 1:
      stateStr = "SUBMITTED";
      break;
    case 2:
      stateStr = "QUEUED";
      break;
    case 3:
      stateStr = "WAITING";
      break;
    case 4:
      stateStr = "RUNNING";
      break;
    case 5:
      stateStr = "TERMINATED";
      break;
    case 6:
      stateStr = "CANCELLED";
      break;
    case 7:
      stateStr = "DOWNLOADED";
      break;
    default:
      stateStr = "UNDEFINED"; 
      break;
  }
  return stateStr;
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

  std::string nameHead = "name";
  std::string memoryHead = "Memory";
  std::string walltimeHead = "Walltime";
  std::string nodeHead = "Node";
  std::string nbRunJobsHead = "Running jobs";
  std::string nbJobsQueHead = "Job in queue"; 
  std::string stateHead = "State";

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

  std::string jobIdHead = "Job id";
  std::string jobNameHead = "Job name";
  std::string ownerHead = "Owner";
  std::string statusHead = "Status";
  std::string queueHead = "Queue";
  std::string priorityHead = "Priority";

  size_t maxJobIdSize = jobIdHead.size();
  size_t maxJobNameSize = jobNameHead.size();
  size_t maxOwnerSize = ownerHead.size();
  size_t maxStatusSize = statusHead.size();
  size_t maxQueueSize = queueHead.size();
  size_t maxPrioritySize = priorityHead.size();

  for(size_t i = 0; i < listJobs.getJobs().size(); i++) {

    jobId = (listJobs.getJobs().get(i))->getJobId();
    maxJobIdSize = std::max(maxJobIdSize, jobId.size());
    
    jobName = (listJobs.getJobs().get(i))->getJobName();
    maxJobNameSize = std::max(maxJobNameSize, jobName.size()); 

    owner = (listJobs.getJobs().get(i))->getOwner();
    maxOwnerSize = std::max(maxOwnerSize, owner.size());

    queue = (listJobs.getJobs().get(i))->getJobQueue();
    maxQueueSize = std::max(maxQueueSize, queue.size());

    status = (listJobs.getJobs().get(i))->getStatus();
    maxStatusSize = std::max(maxStatusSize, convertJobStateToString(status).size());

    priority = (listJobs.getJobs().get(i))->getJobPrio();
    maxPrioritySize = std::max(maxPrioritySize, convertJobPriorityToString(priority).size()+3);
  }

  os << setw(maxJobIdSize+2) << left << jobIdHead << setw(maxJobNameSize+2) << left << jobNameHead << setw(maxOwnerSize+2) ;
  os << left << ownerHead << setw(maxStatusSize+2) << statusHead  << setw(maxQueueSize+2) << left << queueHead;
  os << setw(maxPrioritySize+2) << left << priorityHead  << endl;

  setFill(maxJobIdSize, os);
  setFill(maxJobNameSize, os);
  setFill(maxOwnerSize, os);
  setFill(maxStatusSize, os);
  setFill(maxQueueSize, os);
  setFill(maxPrioritySize, os);
  os << endl;

 for(size_t i = 0; i < listJobs.getJobs().size(); i++) {
 
    jobId = (listJobs.getJobs().get(i))->getJobId();
    jobName = (listJobs.getJobs().get(i))->getJobName();
    owner = (listJobs.getJobs().get(i))->getOwner();
    status = (listJobs.getJobs().get(i))->getStatus();
    queue = (listJobs.getJobs().get(i))->getJobQueue();
    priority = (listJobs.getJobs().get(i))->getJobPrio();

    os << setw(maxJobIdSize+2) << left << jobId;
    os << setw(maxJobNameSize+2) << left << jobName;
    os << setw(maxOwnerSize+2) << left << owner;
    os << setw(maxStatusSize+2) << left << convertJobStateToString(status);
    os << setw(maxQueueSize+2) << left << queue;
    ostringstream oss;
    oss << priority  << "(" << convertJobPriorityToString(priority) << ")";
    os << setw(maxPrioritySize+2) << left << oss.str();
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

  std::string jobIdHead = "Job id";
  std::string jobNameHead = "Job name";
  std::string walltimeHead = "Walltime";
  std::string startTimeHead = "Start time";
  std::string endTimeHead = "End time";
  std::string statusHead = "Status";
  std::string percentHead = "Percent";

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
      pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(startTime));
      maxStartTimeSize = std::max(maxStartTimeSize, boost::posix_time::to_simple_string(pt).size());
    }

    endTime = (listProgress.getProgress().get(i))->getEndTime();
    if(endTime > 0) {
      pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(endTime));
      maxEndTimeSize = std::max(maxEndTimeSize, boost::posix_time::to_simple_string(pt).size());
    }

    status = (listProgress.getProgress().get(i))->getStatus();
    maxStatusSize = std::max(maxStatusSize, convertJobStateToString(status).size());

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
      pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(startTime));
      os << setw(maxStartTimeSize+2) << left << boost::posix_time::to_simple_string(pt);
    } else {
      os << setw(maxStartTimeSize+2) << left << " --- ";
    }
    if(endTime > 0) {
      pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(endTime));
      os << setw(maxEndTimeSize+2) << left << boost::posix_time::to_simple_string(pt);
    } else {
      os << setw(maxEndTimeSize+2) << left <<  " --- ";
    }
    os << setw(maxStatusSize+2) << left <<  convertJobStateToString(status);
    ostringstream oss;
    oss <<  percent << "%";
    os << setw(maxPercentSize+2) << left << oss.str() ;
    os << endl;

  }

  os << endl;
  os << "The number of jobs in queue is: " << listProgress.getNbJobs() << endl;
 
  return os;
}

