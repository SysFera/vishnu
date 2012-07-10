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
   * \param sessionServer The object which encapsulates the session information
   * \param machineId The machine identifier 
   * \brief Constructor
   */
  JobOutputServer(const SessionServer& sessionServer,
                  const std::string& machineId);

  /**
   * \param sessionServer The object which encapsulates the session information
   * \param machineId The machine identifier 
   * \param jobResult The job result data structure
   * \brief Constructor
   */
  JobOutputServer(const SessionServer& sessionServer,
                  const std::string& machineId,
                  const TMS_Data::JobResult& jobResult);
  /**
   * \brief Function to get the job results
   * \return The job results data structure
   */
  TMS_Data::JobResult
  getJobOutput();  

  /**
   * \brief Function to get the all completed jobs results
   * \return The list of job results data structure
   */
  TMS_Data::ListJobResults_ptr
  getCompletedJobsOutput();

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
   * \brief The object which encapsulates the session information 
   */ 
  SessionServer msessionServer;
 
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
  SOCIDatabase *mdatabaseVishnu;  
};

#endif
