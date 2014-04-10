/**
  * \file JobOutputServer.hpp
  * \brief This file contains the VISHNU JobOutputServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */

#ifndef _JOB_OUTPUT_SERVER_H
#define _JOB_OUTPUT_SERVER_H

#include "TMS_Data.hpp"
#include <string>
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "utils.hpp"
/**
 * \class JobOutputServer
 * \brief JobOutputServer class implementation
 */
class JobOutputServer
{

public:
   /**
   * \param sessionServer The object which encapsulates the session information
   * \param machineId The machine identifier
   * \brief Constructor
   */
  JobOutputServer(const std::string& authKey,
                  const std::string& machineId);
  /**
   * \brief Function to get the job results
   * \param options Object containing options
   * \param jobId The job identifier
   * \return The job results data structure
   */
  TMS_Data::JobResult
  getJobOutput(JsonObject* options, const std::string& jobId);

  /**
   * \brief Function to get the all completed jobs results
   * \param options Object containing options
   * \return The list of job results data structure
   */
  TMS_Data::ListJobResults_ptr
  getCompletedJobsOutput(JsonObject* options);

  /**
   * \brief Destructor
   */
  ~JobOutputServer();


  private:
  /**
   * @brief Session info
   */
  std::string mauthKey;

  /**
   * @brief Information about the user and the session
   */
  UserSessionInfo muserSessionInfo;

  /**
   * \brief job results data structure
   */
  TMS_Data::JobResult mjobResult;

  /**
   * \brief The machine identifier
   */
  std::string mmachineId;

  /**
   * \brief The lits of job results data structure
   */
  TMS_Data::ListJobResults_ptr mlistJobsResult;

  /**
   * \brief An instance of vishnu database
   */
  Database *mdatabaseInstance;
};

#endif
