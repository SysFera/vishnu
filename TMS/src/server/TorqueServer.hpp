#ifndef TMS_TORQUE_SERVER_H
#define TMS_TORQUE_SERVER_H

#include "BatchServer.hpp"
#include "pbs_ifl.h"

class TorqueServer : public BatchServer
{
  public:

  TorqueServer();
  int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp=NULL);
  int cancel(const char* jobId) ;
  TMS_Data::Job 
  getJob() { };

  /**
   * \brief To list the jobs on the torque batch
   * \fn TMS_Data::ListJobs* listJobs(ListJobsOptions op)
   * \param op: Options to list the jobs
   * \return The list of the jobs
   */
  TMS_Data::ListJobs*
  listJobs(TMS_Data::ListJobsOptions op);

  TMS_Data::JobResult 
  getJobResults() { };

  TMS_Data::ListQueues* 
  listQueues() { };

  TMS_Data::ListJobResults* 
  getAllJobsResults() { };

  int 
  getJobProgress() {};

  ~TorqueServer(); 

  private:
  /**
   * \brief The number of option for list job
   */
  static const int NBFIELDLISTJOBOPT = 10;
  int
  pbs_cancel(const char* jobId, char remoteServer[], bool isLocal=true);

  void
  process_options(const TMS_Data::SubmitOptions& options, std::vector<std::string>&cmds_options);

  /**
   * \brief To add an option with the corresponding values
   * \fn void fill(struct attrl* attr, bool& first, char* name, char* res, char* val, enum batch_op op)
   * \param attr: The attribute to fill
   * \param first: If it is the first item of the list
   * \param name: The name field value of the attrl struct
   * \param res: The resource field value of the attrl struct
   * \param val: The value field value of the attrl struct
   * \param op: The op field value of the attrl struct
   */
  void
  fill(struct attropl* attr, bool& first, char* name, char* res, char* val, enum batch_op op);

  /**
   * \brief To convert the vishnu listjobsoption to the torque option
   * \fn void makeListJobOption(ListJobsOptions op, struct attrl* attr)
   * \param op: IN, VISHNU options
   * \param attr: OUT, torque options (list stored in an array)
   */
  void
  makeListJobOption(TMS_Data::ListJobsOptions op, struct attropl* attr);

  /**
   * \brief List of jobs returned
   */
  TMS_Data::ListJobs_ptr mjobs;
  /**
   * Server to connect
   */
  char serverOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];
};

#endif
