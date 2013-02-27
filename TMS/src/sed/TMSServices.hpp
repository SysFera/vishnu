/**
 * \file TMSServices.hpp
 * \brief This file lists the services provided by TMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _TMSSERVICES_HPP_
#define _TMSSERVICES_HPP_

/**
 * \brief TMS services enumeration
 */
typedef enum {
  JOBSUBMIT = 0,
  JOBCANCEL,
  JOBINFO,
  GETLISTOFJOBS,
  GETJOBSPROGRESSION,
  GETLISTOFQUEUES,
  JOBOUTPUTGETRESULT,
  JOBOUTPUTGETCOMPLETEDJOBS,
  GETLISTOFJOBS_ALL,
  JOBSUBMIT_AUTOM,
  ADDWORK,
  WORKUPDATE,
  WORKDELETE,
  NB_SRV_TMS  // MUST always be the last
} tms_service_t;



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
  "addwork",  // 10
  "workUpdate",  // 11
  "workDelete"  // 12
};

static const bool MACHINE_SPECIC_SERVICES_TMS[NB_SRV_TMS] = {
  true,   // 0
  true,   // 1
  true,   // 2
  true,   // 3
  true,   // 4
  true,   // 5
  true,   // 6
  true,   // 7
  false,  // 8
  false,  // 9
  false,  // 10
  false,  // 11
  false   // 12
};


#endif  // _TMSSERVICES_HPP_
