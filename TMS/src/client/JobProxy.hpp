/**
  * \file JobProxy.cpp
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
  * \param machine The object which encapsulates the machine information
  * \brief Constructor, raises an exception on error
  */
  explicit JobProxy(const SessionProxy& session,
                    const MachineProxy& machine);
  /**
  * \param session The object which encapsulates the session information
  * \param machine The object which encapsulates the machine information
  * \param job The job data structure
  * \brief Constructor, raises an exception on error
  */
  explicit JobProxy(const SessionProxy& session,
                    const MachineProxy& machine,
                    TMS_Data::Job& job);
  /**
  * \brief Function to submit job
  * \param options the options to submit job
  * \return raises an exception on error
  */
  int
  submitJob(const TMS_Data::SubmitOptions& options);
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
    * \brief Destructor, raises an exception on error
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
  * \brief The object to manipulate the machine data
  */
  MachineProxy mmachineProxy;
  /**
  * \brief The job data structure
  */
  TMS_Data::Job mjob;

};
#endif //_JOB_PROXY_H
