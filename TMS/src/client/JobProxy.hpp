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
  * \param job The job data structure
  * \param options the options to submit job
  * \brief Constructor, raises an exception on error
  */
  explicit JobProxy(SessionProxy session,
                    MachineProxy machine,
                    TMS_Data::Job job,
                    TMS_Data::SubmitOptions options);
  /**
  * \brief Function to submit job
  * \return raises an exception on error
  */
  int
  submitJob();
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
  /**
  * \brief The options data structure to submit job
  */
  TMS_Data::SubmitOptions msubmitOptions;
};
#endif //_JOB_PROXY_H
