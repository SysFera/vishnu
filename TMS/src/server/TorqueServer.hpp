/**
 * \file TorqueServer.hpp
 * \brief This file contains the VISHNU TorqueServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#ifndef TMS_TORQUE_SERVER_H
#define TMS_TORQUE_SERVER_H

#include "BatchServer.hpp"
#include "pbs_ifl.h"

/**
 * \class TorqueServer
 * \brief TorqueServer class implementation
 */
class TorqueServer : public BatchServer
{
  public:

    /**
     * \brief Constructor
     */
    TorqueServer();
   
    /**
     * \brief Function to submit Torque job
     * \param scriptPath the path to the script containing the job characteristique
     * \param options the options to submit job
     * \param job The job data structure
     * \param envp The list of environment variables used by Torque submission function 
     * \return raises an exception on error
     */
    int 
    submit(const char* scriptPath, 
          const TMS_Data::SubmitOptions& options, 
          TMS_Data::Job& job, 
          char** envp=NULL);

    /**
     * \brief Function to cancel job
     * \param jobId the identifier of the job to cancel
     * \return raises an exception on error
     */
    int 
    cancel(const char* jobId) ;

    /**
     * \brief Function to get the status of the job
     * \param jobId the identifier of the job 
     * \return -1 if the job is unknown or server not  unavailable 
     */
    int 
    getJobState(const std::string& jobId);  

    /**
     * \brief Function to get the start time of the job
     * \param jobId the identifier of the job 
     * \return 0 if the job is unknown or server not  unavailable
     */
    time_t 
    getJobStartTime(const std::string& jobId);

   
    /**
     * \brief Function to request the status of queues 
     * \param optQueueName (optional) the name of the queue to request 
     * \return The requested status in to ListQueues data structure 
     */
    TMS_Data::ListQueues*
    listQueues(const std::string& optQueueName=std::string());

    /**
     * \brief Destructor
     */
    ~TorqueServer(); 

  private:
   
    /**
     * \brief Function to cancel job on remote machine
     * \param jobId The identifier of the job
     * \param remoteServer the name of the remote server
     * \param isLocal is the remoteServer is local
     * \return raises an exception on error
     */ 
    int
    pbs_cancel(const char* jobId, 
               char remoteServer[], 
               bool isLocal=true);

    /**
     * \brief Function to treat the submission options
     * \param scriptPath The job script path
     * \param options the object which contains the SubmitOptions options values
     * \param cmdsOptions The list of the option value
     * \return raises an exception on error
     */
    void
    processOptions(const char* scriptPath,
                   const TMS_Data::SubmitOptions& options, 
                   std::vector<std::string>& cmdsOptions);

    /**
     * \brief Function to convert the Torque state into VISHNU state 
     * \param state the state to convert
     * \return VISHNU state 
     */
    int
    convertTorqueStateToVishnuState(std::string state); 

    /**
     * \brief Function to convert the Torque priority into VISHNU priority
     * \param prio the priority to convert
     * \return VISHNU state 
     */
    int
    convertTorquePrioToVishnuPrio(const int& prio); 

    /**
     * \brief Function To fill the info concerning a job
     * \param job: The job to fill
     * \param p: The batch status structure containing the job info
     */
    void
    fillJobInfo(TMS_Data::Job &job, 
                struct batch_status *p);

    /**
     * \brief ListQueues returned
     */
    TMS_Data::ListQueues_ptr mlistQueues; 
    /**
     * \brief The name of the server to connect
     */
    char serverOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];

};

#endif
