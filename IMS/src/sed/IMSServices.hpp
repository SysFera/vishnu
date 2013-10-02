/**
 * \file IMSServices.hpp
 * \brief This file lists the services provided by IMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _IMSSERVICES_HPP_
#define _IMSSERVICES_HPP_

/**
 * \brief TMS services enumeration
 */
typedef enum {
  INT_EXPORTCOMMANDS,
  INT_GETMETRICCURENTVALUE,
  INT_GETMETRICHISTORY,
  INT_GETPROCESSES,
  INT_SETSYSTEMINFO,
  INT_SETSYSTEMTHRESHOLD,
  INT_GETSYSTEMTHRESHOLD,
  INT_LOADSHED,
  INT_SETUPDATEFREQUENCY,
  INT_GETUPDATEFREQUENCY,
  INT_RESTART,
  INT_STOP,
  INT_GETSYSTEMINFO,
  NB_SRV_IMS  // MUST always be the last
} ims_service_t;

static const char* SERVICES_IMS[NB_SRV_IMS] = {
  "int_exportCommands", // 0
  "int_getMetricCurentValue", // 1
  "int_getMetricHistory", // 2
  "int_getProcesses", // 3
  "int_setSystemInfo", // 4
  "int_setSystemThreshold", // 5
  "int_getSystemThreshold", // 6
  "int_loadShed", // 11
  "int_setUpdateFrequency", // 12
  "int_getUpdateFrequency", // 13
  "int_restart", // 14
  "int_stop", // 15
  "int_getSystemInfo" // 16
};

static const bool MACHINE_SPECIC_SERVICES_IMS[NB_SRV_IMS] = {
  false,  // 0
  true,   // 1
  false,  // 2
  false,  // 3
  false,  // 4
  false,  // 5
  false,  // 6
  true,   // 11
  false,  // 12
  false,  // 13
  true,   // 14
  true,   // 15
  false  // 16
};


#endif  // _IMSSERVICES_HPP_
