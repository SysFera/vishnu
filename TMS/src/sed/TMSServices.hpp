/**
 * \file TMSServices.hpp
 * \brief This file lists the services provided by TMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _TMSSERVICES_HPP_
#define _TMSSERVICES_HPP_


/**
 * \brief Number of service in TMS
 */
static const unsigned int NB_SRV_TMS = 11;

static const char* SERVICES_TMS[NB_SRV_TMS] = {
  "jobSubmit",  // 0
  "jobCancel",  // 1
  "jobInfo",    // 2
  "getListOfJobs",  // 3
  "getJobsProgression",  // 4
  "getListOfQueues",  // 5
  "jobOutputGetResult",  // 6
  "jobOutputGetCompletedJobs",  // 7
  "getListOfJobs_all",  // 8
  "jobSubmit_autom",  // 9
  "addwork"  // 10
};

#endif  // _TMSSERVICES_HPP_
