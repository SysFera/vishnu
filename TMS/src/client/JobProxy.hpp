/**
  * \file JobProxy.hpp
  * \brief This file contains the VISHNU JobProxy class.
  * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
  * \date March 2011
  */

#ifndef _JOB_PROXY_H
#define _JOB_PROXY_H

#include "TMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class JobProxy
 * \brief JobProxy class implementation
 */
class JobProxy {

  public:

  /**
  * \param session The object which encapsulates the session information
  * \param machineId The id of the machine
  * \param job The job data structure
  * \brief Constructor
  */
  explicit JobProxy(const SessionProxy& session,
                    const std::string& machineId,
                    TMS_Data::Job& job);
  /**
  * \brief Function to submit job
  * \param scriptContent the content of the script
  * \param options the options to submit job
  * \return raises an exception on error
  */
  int
  submitJob(const std::string scriptContent,
            const TMS_Data::SubmitOptions& options);
  /**
  * \brief Function to cancel job
  * \return raises an exception on error
  */
  int
  cancelJob();
  /**
  * \brief Function to get job information
  * \return The job data structure
  */
  TMS_Data::Job
  getData() const;

  /**
    * \brief Destructor
    */
  ~JobProxy();

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
  * \brief The job data structure
  */
  TMS_Data::Job mjob;

};
#endif //_JOB_PROXY_H
