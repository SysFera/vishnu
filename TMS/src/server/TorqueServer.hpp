#ifndef TMS_TORQUE_SERVER_H
#define TMS_TORQUE_SERVER_H

#include "BatchServer.hpp"
#include "pbs_ifl.h"

class TorqueServer : public BatchServer
{
  public:

  TorqueServer();
  int 
  submit(const char* scriptPath, 
         const TMS_Data::SubmitOptions& options, 
         TMS_Data::Job& job, 
         char** envp=NULL);

  int 
  cancel(const char* jobId) ;
 
  int getJobState(const std::string& jobId);  
  
  time_t getJobStartTime(const std::string& jobId);

  TMS_Data::ListQueues*
  listQueues(const std::string& optQueueName=std::string());

  ~TorqueServer(); 

  private:
  /**
   * \brief The number of option for list job
   */
  static const int NBFIELDLISTJOBOPT = 10;
  int
  pbs_cancel(const char* jobId, char remoteServer[], bool isLocal=true);

  void
  processOptions(const TMS_Data::SubmitOptions& options, std::vector<std::string>& cmdsOptions);

  int convertTorqueStateToVishnuState(std::string state); 

  int convertTorquePrioToVishnuPrio(const int& prio); 
  
  /**
   * \brief To fill the info concerning a job
   * \fn void fillJobInfo(TMS_Data::Job_ptr job, struct batch_status *p)
   * \param job: The job to fill
   * \param p: The batch status structure containing the job info
   */
  void
  fillJobInfo(TMS_Data::Job &job, struct batch_status *p);

  /**
   * \brief List of jobs returned
   */
  TMS_Data::ListJobs_ptr mjobs;
  /**
   * \brief Job returned
   */
  TMS_Data::Job_ptr mjob;
  /**
   * \brief ListQueues returned
   */
  TMS_Data::ListQueues_ptr mlistQueues; 
  /**
   * Server to connect
   */
  char serverOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];
  /**
   * \brief A progression object
   */
  TMS_Data::ListProgression_ptr mprog;
};

#endif
