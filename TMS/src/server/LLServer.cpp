#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <cstdlib>
//#include "llapi.h"
#include "LLServer.hpp"

LLServer::LLServer():BatchServer() {
}

int LLServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

  /*LL_job llJobInfo;
  //options // TODO: A prendre en compte
  if(llsubmit(const_cast<char*>(scriptPath), NULL, NULL, &llJobInfo, LL_JOB_VERSION)) {
    return -1 ;//TODO: A remplacer par l'envoie d'une exception
  };

  std::ostringstream llJobId;
  llJobId<< (llJobInfo.step_list[0])->id.from_host;
  llJobId<< "." << (llJobInfo.step_list[0])->id.cluster;
  llJobId<< "." << (llJobInfo.step_list[0])->id.proc;
  
  job.setJobId(llJobId.str());

  llfree_job_info(&llJobInfo,LL_JOB_VERSION);*/
  
  /**Test sur la machine distante blue gene*/
  return remove_test(job);

 return 0;
}

int LLServer::cancel(const char* jobId) { 

  std::ostringstream cmd;
  //std::string  cancelCommand="llcancel"; //The correct value
  std::string  cancelCommand="ssh bg llcancel";
  
  cmd << cancelCommand << " " << jobId;
  if(system((cmd.str()).c_str())) { //A remplacer par excec
    std::cerr << "LLServer::cancel: can't cancel this job" << std::endl;
    return -1; //Une exception a lever ici
  }
 
 return 0; 
}

int LLServer::remove_test(TMS_Data::Job& job) {
  /**Test sur la machine distante blue gene*/
  std::ostringstream cmd;
  std::string  submitCommand="ssh bg /u/home/ac/ecaron/TEST/APILoadLevelerTest/submittest";
  std::string scriptTest = "/u/home/ac/ecaron/TEST/APILoadLevelerTest/Loadleveler_script2";
  cmd << submitCommand << " " << scriptTest;
  cmd << " > testLL.jobId";

  if(system((cmd.str()).c_str())) { //a remplacer par excec
    std::cerr << "llserver::submit: can't submit the script" << std::endl;
    return -1; //une exception a lever ici  
  }

  std::string jobId;
  ifstream ifile;
  ifile.open("testLL.jobId");

  getline(ifile, jobId);
  getline(ifile, jobId);

  ifile.close();

  if(jobId.size()!=0) {
    std::cout << jobId << std::endl;
    size_t pos = jobId.find(":");
    size_t size = jobId.size();
    jobId = jobId.substr(pos+1, size-pos);
    job.setJobId(jobId);
  }

  return 0;
}
