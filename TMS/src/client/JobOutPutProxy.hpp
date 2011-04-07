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
  * \param outJobResult The job results data structure
  * \return raises an exception on error
  */
  int
	getJobOutPut(const std::string& jobId, TMS_Data::JobResult_ptr& outJobResult);
  /**
  * \brief Function to get the results of all job submitted
  * \param listJobResults_ptr The list of job results
  * \return raises an exception on error
  */
	int
	getAllJobsOutPut(TMS_Data::ListJobResults& listJobResults);
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
