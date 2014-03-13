/**
 * \file PosixServer.hpp
 * \brief This file contains the VISHNU PosixServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2012
 */

#ifndef TMS_POSIX_SERVER_H
#define TMS_POSIX_SERVER_H

#include "BatchServer.hpp"

/**
 * \class PosixServer
 * \brief PosixServer class implementation
 */
class PosixServer : public BatchServer
{
  public:

    /**
     * \brief Constructor
     */
    PosixServer();

    /**
     * \brief Function to submit Posix job
     * \param scriptPath the path to the script containing the job characteristique
     * \param options the options to submit job
     * \param jobSteps List of steps
     * \param envp The list of environment variables used by Posix submission function
     * \return raises an exception on error
     */
    int
      submit(const std::string& scriptPath,
          const TMS_Data::SubmitOptions& options,
          TMS_Data::ListJobs& jobSteps,
          char** envp=NULL);

    /**
     * \brief Function to cancel job
     * \param jobId the identifier of the job to cancel
     * \return raises an exception on error
     */
    int
      cancel(const std::string& jobId) ;

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
        const std::vector<std::string>& ignoredIds=std::vector<std::string>());

    /**
     * \brief Destructor
     */
    ~PosixServer();

  private:

    /**
     * \brief ListQueues returned
     */
    TMS_Data::ListQueues_ptr mlistQueues;
};

#endif
