#ifndef TMS_LL_SERVER_H
#define TMS_LL_SERVER_H

#include <string>

#include "BatchServer.hpp"

class LLServer : public BatchServer
{

public:
 
  LLServer();
 
  int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp=NULL);
 
  int cancel(const char* jobId);
  
  int getJobState(const std::string& jobId);
  
  time_t getJobStartTime(const std::string& jobId);
 
  TMS_Data::ListQueues* listQueues(const std::string& optQueueName=std::string()); 

  ~LLServer();

private:

  int insertOptionLine(const std::string& optionLineToInsert, std::string& content);

  int processOptions(const char* scriptPath, const TMS_Data::SubmitOptions& options);

  int convertLLStateToVishnuState(int state);

  int convertLLPrioToVishnuPrio(const int& prio);

  int computeNbRunJobsAndQueueJobs(std::map<std::string, int>& run, std::map<std::string, int>& que);
  /**
   * \brief List of jobs
   */  
  TMS_Data::ListJobs_ptr mjobs;
  /**
   * \brief List of jobs
   */  
  TMS_Data::ListProgression_ptr mprog;
  /**
   * \brief A job
   */  
  TMS_Data::Job_ptr mjob;
  /**
   * \brief ListQueues returned
   */
  TMS_Data::ListQueues_ptr mlistQueues;
};

#endif
