/**
 * \file UMSServices.hpp
 * \brief This file lists the services provided by UMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _UMSSERVICES_HPP_
#define _UMSSERVICES_HPP_


/**
 * \brief Number of service in UMS
 */
static const unsigned int NB_SRV_UMS = 33;

static const char* SERVICES_UMS[NB_SRV_UMS] = {
  "sessionConnect",  // 0
  "sessionReconnect",  // 1
  "sessionClose",  // 2
  "userCreate",  // 3
  "userUpdate",  // 4
  "userDelete",  // 5
  "userPasswordChange",  // 6
  "userPasswordReset",  // 7
  "machineCreate",  // 8
  "machineUpdate",  // 9
  "machineDelete",  // 10
  "localAccountCreate",  // 11
  "localAccountUpdate",  // 12
  "localAccountDelete",  // 13
  "configurationSave",  // 14
  "configurationRestore",  // 15
  "optionValueSet",  // 16
  "optionValueSetDefault",  // 17
  "sessionList",  // 18
  "localAccountList",  // 19
  "machineList",  // 20
  "commandList",  // 21
  "optionValueList",  // 22
  "userList",  // 23
  "restore",  // 24
  "authSystemCreate",  // 25
  "authSystemUpdate",  // 26
  "authSystemDelete",  // 27
  "authSystemList",  // 28
  "authAccountCreate",  // 29
  "authAccountUpdate",  // 30
  "authAccountDelete",  // 31
  "authAccountList"  // 32
};

#endif  // _UMSSERVICES_HPP_
