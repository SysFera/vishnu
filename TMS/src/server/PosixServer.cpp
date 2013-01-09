#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include "PosixServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"



PosixServer::PosixServer():BatchServer(){
}


int
PosixServer::submit(const char* scriptPath,
                    const TMS_Data::SubmitOptions& options,
                    TMS_Data::Job& job,
                    char** envp){
  return system(scriptPath);
}


int
PosixServer::cancel(const char* jobId){
  return 0;
}


int
PosixServer::getJobState(const std::string& jobId){
  return 5;
}


time_t
PosixServer::getJobStartTime(const std::string& jobId){
  return time_t();
}


TMS_Data::ListQueues*
PosixServer::listQueues(const std::string& optQueueName){
  return new TMS_Data::ListQueues();
}


void
PosixServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                            const std::vector<std::string>& ignoredIds){
}


PosixServer::~PosixServer(){
}

