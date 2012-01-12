/**
 * \file BatchServer.hpp
 * \brief This file contains the VISHNU BatchServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#ifndef TMS_BATCH_SERVER_H 
#define TMS_BATCH_SERVER_H

#include <string>
#include <iostream>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "TMS_Data.hpp"

using namespace std;

/**
 * \class BatchServer
 * \brief BatchServer class implementation
 */
class BatchServer
{
public:

  /**
   * \brief Constructor
   */
  BatchServer();
   
  /**
   * \brief Function to submit a job
   * \param scriptPath the path to the script containing the job characteristique
   * \param options the options to submit job
   * \param job The job data structure
   * \param envp The list of environment variables used by submission function 
   * \return raises an exception on error
   */
  virtual int 
  submit(const char* scriptPath, 
         const TMS_Data::SubmitOptions& options, 
         TMS_Data::Job& job, char** envp=NULL)=0;

  /**
   * \brief Function to cancel job
   * \param jobId the identifier of the job to cancel
   * \return raises an exception on error
   */  
  virtual int 
  cancel(const char* jobId)=0;

  /**
   * \brief Function to get the status of the job
   * \param jobId the identifier of the job 
   * \return -1 if the job is unknown or server not  unavailable 
   */
  virtual int 
  getJobState(const std::string& jobId)=0;
  
  /**
   * \brief Function to get the start time of the job
   * \param jobId the identifier of the job 
   * \return 0 if the job is unknown or server not  unavailable
   */
  virtual time_t 
  getJobStartTime(const std::string& jobId)=0;
  
  /**
   * \brief Function to request the status of queues 
   * \param optQueueName (optional) the name of the queue to request 
   * \return The requested status in to ListQueues data structure 
   */
  virtual TMS_Data::ListQueues* 
  listQueues(const std::string& optQueueName=std::string())=0;

   /**
   * \brief Function to get a list of submitted jobs
   * \param listOfJobs the ListJobs structure to fill
   * \param ignoredIds the list of job ids to ignore 
   */
  virtual void fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                      const std::vector<string>& ignoredIds=std::vector<string>())=0;
   
  /**
   * \brief Destructor
   */
  virtual ~BatchServer();

private:
};

#endif
