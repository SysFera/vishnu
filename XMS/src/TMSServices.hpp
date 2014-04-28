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
  GETJOBSPROGRESSION,
  GETLISTOFQUEUES,
  JOBOUTPUTGETRESULT,
  JOBOUTPUTGETCOMPLETEDJOBS,
  GETLISTOFJOBS_ALL,
  ADDWORK,
  WORKUPDATE,
  WORKDELETE,
  HANDLE_SCHEDULING,
  NB_SRV_TMS  // MUST always be the last
} tms_service_t;



static const char* SERVICES_TMS[NB_SRV_TMS] = {
  "jobSubmit",  // 0
  "jobCancel",  // 1
  "jobInfo",    // 2
  "getJobsProgression",  // 3
  "getListOfQueues",  // 4
  "jobOutputGetResult",  // 5
  "jobOutputGetCompletedJobs",  // 6
  "getListOfJobs_all",  // 7
  "addwork",  // 8
  "workUpdate",  // 9
  "workDelete",  // 10
  "scheduling"  // 11
};



// FIXME: compilation fails without inlining
// needs to be moved in an implementation file
inline bool
isMachineSpecificServicesTMS(unsigned id) {
    bool machineLocal = (id <= 7) ? true : false;
  return machineLocal;
}

#endif  // _TMSSERVICES_HPP_
