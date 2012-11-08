/**
 * \file SlurmServer.hpp
 * \brief This file contains the VISHNU SlurmServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date July 2011
 */

#ifndef TMS_SLURM_SERVER_H
#define TMS_SLURM_SERVER_H

#include "BatchServer.hpp"

/**
 * \class SlurmServer
 * \brief SlurmServer class implementation
 */
class SlurmServer : public BatchServer
{
  public:

    /**
     * \brief Constructor
     */
    SlurmServer();

    /**
     * \brief Function to submit Slurm job
     * \param scriptPath the path to the script containing the job characteristique
     * \param options the options to submit job
     * \param job The job data structure
     * \param envp The list of environment variables used by Slurm submission function
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
    ~SlurmServer();

  protected:

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
     * \brief Function to convert the Slurm state into VISHNU state
     * \param state the state to convert
     * \return VISHNU state
     */
    int
    convertSlurmStateToVishnuState(const uint16_t& state);

    /**
     * \brief Function to convert the Slurm priority into VISHNU priority
     * \param prio the priority to convert
     * \return VISHNU state
     */
    int
    convertSlurmPrioToVishnuPrio(const uint32_t& prio);

    /**
     * \brief Function To fill the info concerning a job
     * \param job: The job to fill
     * \param jobId: The identifier of the job to load
     */
    void
    fillJobInfo(TMS_Data::Job &job, const uint32_t& jobId);

    /**
     * \brief Function To convert vishnu job Id to slurm job Id
     * \param jobId: vishnu job Id
     * \return the converted slurm job id
     */
    uint32_t
    convertToSlurmJobId(const std::string& jobId);

    /**
     * \brief Function to compute the number of running and waiting jobs of each queue
     * \param run contains the number of running jobs of each queue
     * \param que contains the number of waiting jobs of each queue
     * \return non zero if error
     */
    int
    computeNbRunJobsAndQueueJobs(std::map<std::string, size_t>& run,
          std::map<std::string, size_t>& que);

    /**
     * \brief Function to replace slurm job identifer symbol by its real value in to a path
     * \param path The path containing the job symbol
     */
    void
    replaceSymbolInToJobPath(std::string& path);

    /**
     * \brief Function to cheick if a path contains an excluded slurm symbol by vishnu
     * \param path The path to check
     * \param symbol The excluded symbol
     * \return true if the path contain an exlude symbol
     */
    bool
    containsAnExcludedSlurmSymbols(const std::string& path, std::string& symbol);

    /**
     * \brief Function to check if slurm path syntax is correct
     * \param path the path to check
     * \param pathInfo The information on path to print
     * \return an error message
     */
    std::string
    checkSLURMOutPutPath(char*& path, const std::string& pathInfo="job output path");

    /**
     * \brief Function to get the value of SLURM resource (nodes, walltime, cpu) in the script
     * \param file The file contain the script to scan
     * \param shortOptionLetterSyntax The syntax of the short option (--N, -t) whose value will be returned
     * \param longOptionLetterSyntax The syntax of the long option (--nodes, --time) whose value will be returned
     * \return The value of the resource
     */
    std::string
    getSlurmResourceValue(const char* file,
          const std::string& shortOptionLetterSyntax,
          const std::string& longOptionLetterSyntax);

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
