/**
 * \file IMSServices.hpp
 * \brief This file lists the services provided by IMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _IMSSERVICES_HPP_
#define _IMSSERVICES_HPP_

/**
 * \brief Number of service in IMS
 */
static const unsigned int NB_SRV_IMS = 19;

static const char* SERVICES_IMS[NB_SRV_IMS] = {
  "int_exportCommands", // 0
  "int_getMetricCurentValue", // 1
  "int_getMetricHistory", // 2
  "int_getProcesses", // 3
  "int_setSystemInfo", // 4
  "int_setSystemThreshold", // 5
  "int_getSystemThreshold", // 6
  "int_defineUserIdentifier", // 7
  "int_defineJobIdentifier", // 8
  "int_defineTransferIdentifier", // 9
  "int_defineMachineIdentifier", // 10
  "int_loadShed", // 11
  "int_setUpdateFrequency", // 12
  "int_getUpdateFrequency", // 13
  "int_restart", // 14
  "int_stop", // 15
  "int_getSystemInfo", // 16
  "int_defineAuthIdentifier", // 17
  "int_defineWorkIdentifier" // 18
};

static const bool MACHINE_SPECIC_SERVICES_IMS[NB_SRV_IMS] = {
  false,  // 0
  true,   // 1
  false,  // 2
  false,  // 3
  false,  // 4
  false,  // 5
  false,  // 6
  false,  // 7
  false,  // 8
  false,  // 9
  false,  // 10
  true,   // 11
  false,  // 12
  false,  // 13
  true,   // 14
  true,   // 15
  false,  // 16
  false,  // 17
  false   // 18
};


#endif  // _IMSSERVICES_HPP_
