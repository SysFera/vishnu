/**
  * \file JobServer.hpp
  * \brief This file contains the VISHNU JobServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */

#ifndef _JOB_SERVER_H
#define _JOB_SERVER_H

#include "TMS_Data.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
/**
 * \class JobServer
 * \brief JobServer class implementation
 */
class JobServer
{
 
public:
	/**
  * \param session The object which encapsulates the session information
  * \param machineid The machine identifier 
  * \param job The job data structure
  * \brief Constructor
  */
  explicit JobServer(const SessionServer& sessionServer,
                     const std::string& machineId,
                     const TMS_Data::Job& job,
                     const BatchType& batchType);
  /**
   * \brief Function to submit job
   * \param scriptContent the content of the script
   * \param options the options to submit job
   * \return raises an exception on error
   */
	int submitJob(const std::string& scriptContent, const TMS_Data::SubmitOptions& options);

  /**
   * \brief Function to cancel job
   * \return raises an exception on error
   */
	int cancelJob();
	
  /**
   * \brief Function to get job information
   * \return The job data structure
   */
  TMS_Data::Job getData();


  /**
   * \brief Destructor
   */
  ~JobServer(); 
  

  private:

  void scanErrorMessage(const std::string& errorInfo, int& code, std::string& message);

  TMS_Data::Job mjob;
  SessionServer msessionServer;
  std::string mmachineId;
  BatchType mbatchType;
};

#endif
