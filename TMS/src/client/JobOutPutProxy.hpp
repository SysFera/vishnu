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

#include "TMS_Data/ListJobResults.hpp"
#include "TMS_Data/JobResult.hpp"

/**
 * \class JobOutPutProxy
 * \brief JobOutPutProxy class implementation
 */

class JobOutPutProxy {

public:

  /**
  * \brief Constructor, raises an exception on error
  */
  /*explicit
  JobOutPutProxy();
  */
  /**
  * \param session The object which encapsulates the session information
  * \param machine The object which encapsulates the machine information
  * \param jobResult The job results data structure
  * \param listJobResults the list of job results data structure
  * \brief Constructor, raises an exception on error
  */
	explicit
	JobOutPutProxy( SessionProxy session,
                  MachineProxy machine,
                  TMS_Data::JobResult result,
                  TMS_Data::ListJobResults* listJobResults);

  /**
  * \brief Function to get the job results
  * \param jobId The identifier of a job
  * \return raises an exception on error
  */
	TMS_Data::JobResult
	getJobOutPut(std::string jobId);
  /**
  * \brief Function to get the results of all job submitted
  * \return raises an exception on error
  */
	TMS_Data::ListJobResults*
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
  * \brief The object to manipulate the machine data
  */
  MachineProxy mmachineProxy;
  /**
  * \brief The Job results data structure
  */
  TMS_Data::JobResult mjobResult;
  /**
  * \brief the list of job results data structure
  */
  TMS_Data::ListJobResults* mlistJobResults;
};

#endif
