/**
 * \file LSFServer.hpp
 * \brief This file contains the VISHNU LSFServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date July 2011
 */

#ifndef TMS_SLURM_SERVER_H
#define TMS_SLURM_SERVER_H

#include "BatchServer.hpp"

/**
 * \class LSFServer
 * \brief LSFServer class implementation
 */
class LSFServer : public BatchServer
{
  public:

    /**
     * \brief Constructor
     */
    LSFServer();
   
    /**
     * \brief Function to submit LSF job
     * \param scriptPath the path to the script containing the job characteristique
     * \param options the options to submit job
     * \param job The job data structure
     * \param envp The list of environment variables used by LSF submission function 
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
     * \return 0 if the job is unknown 
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
     * \brief Function to get a list of submitted jobs
     * \param listOfJobs the ListJobs structure to fill
     * \param ignoredIds the list of job ids to ignore 
     */
    void fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
        const std::vector<string>& ignoredIds=std::vector<string>());

    /**
     * \brief Destructor
     */
    ~LSFServer(); 

  private:
   
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
     * \brief Function to convert the LSF state into VISHNU state 
     * \param state the state to convert
     * \return VISHNU state 
     */
    int
    convertLSFStateToVishnuState(const uint16_t& state); 

    /**
     * \brief Function to convert the LSF priority into VISHNU priority
     * \param prio the priority to convert
     * \return VISHNU state 
     */
    int
    convertLSFPrioToVishnuPrio(const uint32_t& prio); 

    /**
     * \brief Function To fill the info concerning a job
     * \param job: The job to fill
     * \param jobId: The identifier of the job to load
     */
    void
    fillJobInfo(TMS_Data::Job &job, const uint32_t& jobId);

    /**
     * \brief Function To convert vishnu job Id to LSF job Id 
     * \param jobId: vishnu job Id
     * \return the converted LSF job id
     */  
    uint32_t
    convertToLSFJobId(const std::string& jobId);

    /**
     * \brief ListQueues returned
     */
    TMS_Data::ListQueues_ptr mlistQueues;

};

#endif
