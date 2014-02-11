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
  JobOutputServer(const std::string& machineId);

  /**
   * \param machineId The machine identifier
   * \param jobResult The job result data structure
   * \brief Constructor
   */
  JobOutputServer(const std::string& machineId, const TMS_Data::JobResult& jobResult);
  /**
   * \brief Function to get the job results
   * \param options Object containing options
   * \return The job results data structure
   */
  TMS_Data::JobResult
  getJobOutput(JsonObject* options);

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
  Database *mdatabaseVishnu;
};

#endif
