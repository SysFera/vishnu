#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "llapi.h"
#include "LLServer.hpp"

LLServer::LLServer():BatchServer() {
}

int LLServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

  LL_job llJobInfo;
  //options // TODO: A prendre en compte
  if(llsubmit(const_cast<char*>(scriptPath), NULL, NULL, &llJobInfo, LL_JOB_VERSION)) {
    return -1 ;//TODO: A remplacer par l'envoie d'une exception
  };

  std::ostringstream llJobId;
  llJobId<< (llJobInfo.step_list[0])->id.from_host;
  llJobId<< (llJobInfo.step_list[0])->id.cluster;
  llJobId<< (llJobInfo.step_list[0])->id.proc;
  
  job.setJobId(llJobId.str());

  llfree_job_info(&llJobInfo,LL_JOB_VERSION);
}

int cancel(const char* jobId) { 

  std::ostringstream cmd;
  std::string  cancelCommand="/opt/ibmll/LoadL/full/bin/llcancel";
  
  cmd << cancelCommand << " " << jobId;
  if(system((cmd.str()).c_str())) { //A remplacer par excec
    return -1; //Une exception a lever ici  
  }
 
 return 0; 
}
