/**
 * \file UMSServices.hpp
 * \brief This file lists the services provided by UMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _UMSSERVICES_HPP_
#define _UMSSERVICES_HPP_


/**
 * \brief UMS services enumeration
 */
typedef enum {
  SESSIONCONNECT = 0,
  SESSIONRECONNECT,
  SESSIONCLOSE,
  USERCREATE,
  USERUPDATE,
  USERDELETE,
  USERPASSWORDCHANGE,
  USERPASSWORDRESET,
  MACHINECREATE,
  MACHINEUPDATE,
  MACHINEDELETE,
  LOCALACCOUNTCREATE,
  LOCALACCOUNTUPDATE,
  LOCALACCOUNTDELETE,
  SESSIONLIST,
  LOCALACCOUNTLIST,
  MACHINELIST,
  COMMANDLIST,
  OPTIONVALUELIST,
  USERLIST,
  RESTORE,
  AUTHSYSTEMCREATE,
  AUTHSYSTEMUPDATE,
  AUTHSYSTEMDELETE,
  AUTHSYSTEMLIST,
  AUTHACCOUNTCREATE,
  AUTHACCOUNTUPDATE,
  AUTHACCOUNTDELETE,
  AUTHACCOUNTLIST,
  EXPORT,
  NB_SRV_UMS  // MUST always be the last
} ums_service_t;

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
  "sessionList",  // 14
  "localAccountList",  // 15
  "machineList",  // 16
  "commandList",  // 17
  "optionValueList",  // 18
  "userList",  // 19
  "restore",  // 20
  "authSystemCreate",  // 21
  "authSystemUpdate",  // 22
  "authSystemDelete",  // 23
  "authSystemList",  // 24
  "authAccountCreate",  // 25
  "authAccountUpdate",  // 26
  "authAccountDelete",  // 27
  "authAccountList",  // 28
  "exportCommands" // 29
};

// FIXME: compilation fails without inlining
// needs to be moved in an implementation file
inline bool
isMachineSpecificServicesUMS(unsigned id) {
  return false;
}


#endif  // _UMSSERVICES_HPP_
