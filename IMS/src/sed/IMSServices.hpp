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


#endif  // _IMSSERVICES_HPP_
