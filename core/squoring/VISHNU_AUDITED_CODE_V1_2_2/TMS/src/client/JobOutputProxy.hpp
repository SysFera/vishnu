/**
* \file JobOutputProxy.hpp
* \brief This file contains the VISHNU JobOutputProxy class.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date March 2011
*/

#ifndef _JOB_OUT_PUT_PROXY_H
#define _JOB_OUT_PUT_PROXY_H

#include "TMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class JobOutputProxy
 * \brief JobOutputProxy class implementation
 */

class JobOutputProxy {

public:

  /**
  * \param session The object which encapsulates the session information
  * \param machineId The machine identifier where has been submitted
  * \param outDir The output directory where the files will be stored 
  * (default is current directory) 
  * \brief Constructor, raises an exception on error
  */
	explicit
	JobOutputProxy( const SessionProxy& session,
                  const std::string& machineId,
                  const std::string& outDir);

  /**
  * \brief Function to get the job results
  * \param jobId The Id of the
  * \return The job results data structure
  */
  //TMS_Data::JobResult_ptr
  TMS_Data::JobResult
	getJobOutPut(const std::string& jobId);

  /**
  * \brief Function to get the results of all job submitted
  * \return The list of the job results
  */
	TMS_Data::ListJobResults_ptr
	getCompletedJobsOutput();

  /**
  * \brief Destructor, raises an exception on error
  */
  ~JobOutputProxy();

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

  /**
  * \brief The output directory where the files will be stored
  */ 
  std::string moutDir;
};

#endif
