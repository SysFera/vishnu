/**
 * \file SGEServer.hpp
 * \brief This file contains the VISHNU SGEServer class.
 * \author Amine Bsila (amine.bsila@sysfera.com)
 * \date Febrary 2012
 */

#ifndef TMS_SGE_SERVER_H
#define TMS_SGE_SERVER_H

#include "BatchServer.hpp"
extern "C" {
#include "drmaa.h"
}

/**
 * \class SGEServer
 * \brief SGEServer class implementation
 */
class SGEServer : public BatchServer
{
  public:

    /**
     * \brief Constructor
     */
    SGEServer();
   
    /**
     * \brief Function to submit SGE job
     * \param scriptPath the path to the script containing the job characteristique
     * \param options the options to submit job
     * \param job The job data structure
     * \param envp The list of environment variables used by SGE submission function 
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
    ~SGEServer(); 

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
                    std::vector<std::string>&cmdsOptions,
                    drmaa_job_template_t *jobt);

    /**
    * \brief Function to get the script submission options
    * \param scriptPath The job script path
    * \param cmdsOptions The list of the option value
    * \return raises an exception on error
    */
    void
    getScriptOptions(const char* scriptPath,
                     std::vector<std::string>& cmdsOptions);
    /**
     * \brief Function to get the value of SGE resource (nodes, walltime, cpu) in the script
     * \param file The file contain the script to scan
     * \param shortOptionLetterSyntax The syntax of the short option (--N, -t) whose value will be returned
     * \param longOptionLetterSyntax The syntax of the long option (--nodes, --time) whose value will be returned
     * \return The value of the resource
     */
    std::string
    getSGEResourceValue(const char* file,
          const std::string& shortOptionLetterSyntax,
          const std::string& longOptionLetterSyntax);
    
    /**
     * \brief Function To fill the info concerning a job
     * \param job: The job to fill
     * \param jobId: The identifier of the job to load
     */
    /*void
    fillJobInfo(TMS_Data::Job &job, const uint32_t& jobId);*/

    
    /**
     * \brief Function to compute the number of running and waiting jobs of each queue 
     * \param run contains the number of running jobs of each queue 
     * \param que contains the number of waiting jobs of each queue
     * \return non zero if error
     */
    /*int
    computeNbRunJobsAndQueueJobs(std::map<std::string, size_t>& run,
          std::map<std::string, size_t>& que);*/

    /**
     * \brief Function to replace slurm job identifer symbol by its real value in to a path
     * \param path The path containing the job symbol
     */
    /*void 
    replaceSymbolInToJobPath(std::string& path); */

        
    /**
     * \brief ListQueues returned
     */
    TMS_Data::ListQueues_ptr mlistQueues;

    /**
     * \brief msymbolMap contains the slurm partern symbols of job output and error path
     */
    std::map<std::string, std::string> msymbolMap; 
};

#endif

