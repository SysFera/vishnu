
#include "boost/date_time/posix_time/posix_time.hpp"
#include "utilVishnu.hpp"
#include "displayer.hpp"

using namespace std;
using namespace vishnu;

void 
displayAllJobOutput(TMS_Data::ListJobResults& j){
  for (unsigned int i = 0 ; i < j.getResults().size() ; i++){
    displayJobOutput(j.getResults().get(i));
  }
}

void 
displayJobOutput(TMS_Data::JobResult_ptr j){
  cout << " ------------------------ " << endl;
  cout << " Job Id     : " << j->getJobId() << endl;
  cout << " Output path: " << j->getOutputPath() << endl;
  cout << " Error  path: " << j->getErrorPath() << endl;
  cout << endl;
}

void 
displayJob(TMS_Data::Job& j){

  boost::posix_time::ptime pt;

  cout << " ------------------------ " << endl;
  cout << " Session Id      : " << j.getSessionId() << endl;
  cout << " Machine Id      : " << j.getSubmitMachineId() << endl;
  cout << " Machine name    : " << j.getSubmitMachineName() << endl;
  cout << " Job Id          : " << j.getJobId() << endl;
  cout << " Job name        : " << j.getJobName() << endl;
  cout << " Job path        : " << j.getJobPath() << endl;
  cout << " Output path     : " << j.getOutputPath() << endl;
  cout << " Error path      : " << j.getErrorPath() << endl;
  cout << " Script content  : " << j.getScriptContent() << endl;
  cout << " Priority        : " << j.getJobPrio() << endl;
  cout << " CPU             : " << j.getNbCpus() << endl;
  cout << " Working dir     : " << j.getJobWorkingDir() << endl;
  cout << " Status          : " << j.getStatus() << endl;
  pt =  boost::posix_time::from_time_t(j.getSubmitDate());
  cout << " Submit date     : " << boost::posix_time::to_simple_string(pt) << endl;
  pt =  boost::posix_time::from_time_t(j.getEndDate());
  cout << " End date        : " << boost::posix_time::to_simple_string(pt) << endl;
  cout << " Owner           : " << j.getOwner() << endl;
  cout << " Queue           : " << j.getJobQueue() << endl;
  cout << " Wall clock limit: " << convertWallTimeToString(j.getWallClockLimit()) << endl;
  cout << " Group name      : " << j.getGroupName() << endl;
  cout << " Description     : " << j.getJobDescription() << endl;
  cout << " Max memory      : " << j.getMemLimit() << endl;
  cout << " Nodes           : " << j.getNbNodes() << endl;
  cout << " CPU/Node        : " << j.getNbNodesAndCpuPerNode() << endl;
  cout << endl;
}

void 
displayJobProgress(ListProgression& j){
  for (unsigned int i = 0 ; i < j.getProgress().size() ; i++){
    displayProgress(*(j.getProgress().get(i)));
  }
}

void
displayProgress(Progression& p){

  boost::posix_time::ptime pt;

  cout << " ------------------------ " << endl;
  cout << " Job Id    : " << p.getJobId() << endl;
  cout << " Job name  : " << p.getJobName() << endl;
  cout << " Wall time : " << convertWallTimeToString(p.getWallTime()) << endl;
  pt =  boost::posix_time::from_time_t(p.getStartTime());
  cout << " Start time: " << boost::posix_time::to_simple_string(pt) << endl;
  pt =  boost::posix_time::from_time_t(p.getEndTime());
  cout << " End time  : " << boost::posix_time::to_simple_string(pt) << endl;
  cout << " Percent   : " << p.getPercent() << "%" << endl;
  cout << " Status    : " << p.getStatus() << endl;
  cout << endl;
}

void 
displayRefreshPeriod(int& j){
  cout << "The refresh period is " << j << " seconds " << endl;
}

void 
displayListJobs(ListJobs& j){
  for (unsigned int i = 0 ; i < j.getJobs().size() ; i++){
    displayJob(*(j.getJobs().get(i)));
  }  
}

void 
displayQueues(ListQueues& j){
  for (unsigned int i = 0 ; i < j.getQueues().size() ; i++){
    displayQueue(*(j.getQueues().get(i)));
  }  
}


void
displayQueue(Queue& q){

  boost::posix_time::ptime pt;

  cout << " ------------------------ " << endl;
  cout << " Name        : " << q.getName() << endl;
  cout << " Max job cpu : " << q.getMaxJobCpu() << endl;
  cout << " Max proc cpu: " << q.getMaxProcCpu() << endl;
  cout << " Memory      : " << q.getMemory() << endl;
  cout << " Wall time   : " << convertWallTimeToString(q.getWallTime()) << endl;
  cout << " Node        : " << q.getNode() << endl;
  cout << " Running jobs: " << q.getNbRunningJobs() << endl;
  cout << " Job in queue: " << q.getNbJobsInQueue() << endl;
  cout << " State       : " << q.getState() << endl;
  cout << " Priority    : " << q.getPriority() << endl;
  cout << " Description : " << q.getDescription() << endl;
}


void
displaySubmit(TMS_Data::Job job){
  cout << "Job Id     : " << job.getJobId() << endl;
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
 * \brief Helper function to display a list of users
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
  int state;

  std::string nameHead = "name";
  std::string memoryHead = "Memory";
  std::string walltimeHead = "Walltime";
  std::string nodeHead = "Node";
  std::string nbRunJobsHead = "Running jobs";
  std::string nbJobsQueHead = "Job in queue"; 
  std::string stateHead = "State";
  std::string blank = " --- ";

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
    os << setw(maxMemorySize+2) << left << Memory;
    os << setw(maxWalltimeSize+2) << left << vishnu::convertWallTimeToString(walltime);
    os << setw(maxNodeSize+2) << left << node;
    os << setw(maxNbRunJobsSize+2) << left << nbRunJobs;
    os << setw(maxNbJobsQueSize+2) << left << nbJobsQue;
    os << setw(maxStateSize+2) << left << state;
    os << endl;

  } 

  return os;
}

