/**
  * \file JobServer.cpp
  * \brief This file contains the VISHNU JobServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */
#include "JobServer.hpp"

/**
 * \param session The object which encapsulates the session information
 * \param machine The object which encapsulates the machine information 
 * \param job The job data structure
 * \brief Constructor
 */
JobServer::JobServer(const SessionServer& session, 
                     const std::string& machineId, 
                     const TMS_Data::Job& job, 
                     const BatchType& batchType)
{
}

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \return raises an exception on error
 */ 
int JobServer::submitJob(const std::string& scriptContent, const TMS_Data::SubmitOptions& options)
{
	return 0;
}

/**
 * \brief Function to cancel job
 * \return raises an exception on error
 */ 
int JobServer::cancelJob()
{
	return 0;
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */ 
TMS_Data::Job JobServer::getData()
{
	return mjob;
}

/**
 * \brief Destructor
 */
JobServer::~JobServer() {

}
