/**
* \file JobOutPutProxy.cpp
* \brief This file contains the VISHNU JobOutPutProxy class.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date March 2011
*/

#ifndef _JOB_OUT_PUT_PROXY_H
#define _JOB_OUT_PUT_PROXY_H

#include "TMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class JobOutPutProxy
 * \brief JobOutPutProxy class implementation
 */

class JobOutPutProxy {

public:

  /**
  * \param session The object which encapsulates the session information
  * \param machine The object which encapsulates the machine information
  * \param jobResult The job results data structure
  * \param listJobResults the list of job results data structure
  * \brief Constructor, raises an exception on error
  */
	explicit
	JobOutPutProxy( const SessionProxy& session,
                  const std::string& machineId);

  /**
  * \brief Function to get the job results
  * \param jobId The Id of the
  * \return The job results data structure
  */
  TMS_Data::JobResult_ptr
	getJobOutPut(const std::string& jobId);

  /**
  * \brief Function to get the results of all job submitted
  * \return The list of the job results
  */
	TMS_Data::ListJobResults_ptr
	getAllJobsOutPut();

  /**
  * \brief Destructor, raises an exception on error
  */
  ~JobOutPutProxy();

private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The object to manipulate the session data
  */
  SessionProxy msessionProxy;
  /**
  * \brief The id of the machine
  */
  std::string mmachineId;
};

#endif
