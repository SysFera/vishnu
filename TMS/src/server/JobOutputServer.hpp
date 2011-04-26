/**
  * \file JobOutputServer.hpp
  * \brief This file contains the VISHNU JobOutputServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */

#ifndef _JOB_OUTPUT_SERVER_H
#define _JOB_OUTPUT_SERVER_H

#include "TMS_Data.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
/**
 * \class JobOutputServer
 * \brief JobOutputServer class implementation
 */
class JobOutputServer
{
 
public:
   /**
   * \param session The object which encapsulates the session information
   * \param machineid The machine identifier 
   * \brief Constructor
   */
  JobOutputServer(const SessionServer& sessionServer,
                  const std::string& machineId);

  /**
   * \param session The object which encapsulates the session information
   * \param machineid The machine identifier 
   * \param jobResult The job result data structure
   * \brief Constructor
   */
  JobOutputServer(const SessionServer& sessionServer,
                  const std::string& machineId,
                  const TMS_Data::JobResult& jobResult);
  /**
   * \brief Function to get the job results
   * \param jobId The Id of the
   * \return The job results data structure
   */
  TMS_Data::JobResult
  getJobOutput();  

  /**
   * \brief Function to get the job results
   * \param jobId The Id of the
   * \return The lits of job results data structure
   */
  TMS_Data::ListJobResults_ptr
  getCompletedJobsOutput();

  /**
   * \brief Destructor
   */
  ~JobOutputServer(); 
  

  private:

  TMS_Data::JobResult mjobResult;
  SessionServer msessionServer;
  std::string mmachineId;
  TMS_Data::ListJobResults_ptr mlistJobsResult;
};

#endif
