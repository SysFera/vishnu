#include "displayer.hpp"

using namespace std;

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
  cout << " Submit date     : " << j.getSubmitDate() << endl;
  cout << " End date        : " << j.getEndDate() << endl;
  cout << " Owner           : " << j.getOwner() << endl;
  cout << " Queue           : " << j.getJobQueue() << endl;
  cout << " Wall clock limit: " << j.getWallClockLimit() << endl;
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
displayProgress(Progression& j){
  cout << " ------------------------ " << endl;
  cout << " Job Id    : " << j.getJobId() << endl;
  cout << " Job name  : " << j.getJobName() << endl;
  cout << " Wall time : " << j.getWallTime() << endl;
  cout << " Start time: " << j.getStartTime() << endl;
  cout << " End time  : " << j.getEndTime() << endl;
  cout << " Percent   : " << j.getPercent() << "%" << endl;
  cout << " Status    : " << j.getStatus() << endl;
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
displayQueue(Queue& j){
  cout << " ------------------------ " << endl;
  cout << " Name        : " << j.getName() << endl;
  cout << " Max job cpu : " << j.getMaxJobCpu() << endl;
  cout << " Max proc cpu: " << j.getMaxProcCpu() << endl;
  cout << " Memory      : " << j.getMemory() << endl;
  cout << " Wall time   : " << j.getWallTime() << endl;
  cout << " Node        : " << j.getNode() << endl;
  cout << " Running jobs: " << j.getNbRunningJobs() << endl;
  cout << " Job in queue: " << j.getNbJobsInQueue() << endl;
  cout << " State       : " << j.getState() << endl;
  cout << " Priority    : " << j.getPriority() << endl;
  cout << " Description : " << j.getDescription() << endl;
}


void
displaySubmit(TMS_Data::Job job){
  cout << "Job Id     : " << job.getJobId() << endl;
  cout << " Session Id: " << j.getSessionId() << endl;
  cout << " Machine Id: " << j.getSubmitMachineId() << endl;
}
