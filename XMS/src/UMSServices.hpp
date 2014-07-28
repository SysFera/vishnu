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
  "sessionConnect",
  "sessionReconnect",
  "sessionClose",
  "userCreate",
  "userUpdate",
  "userDelete",
  "userPasswordChange",
  "userPasswordReset",
  "machineCreate",
  "machineUpdate",
  "machineDelete",
  "localAccountCreate",
  "localAccountUpdate",
  "localAccountDelete",
  "sessionList",
  "localAccountList",
  "machineList",
  "commandList",
  "userList",
  "restore",
  "authSystemCreate",
  "authSystemUpdate",
  "authSystemDelete",
  "authSystemList",
  "authAccountCreate",
  "authAccountUpdate",
  "authAccountDelete",
  "authAccountList",
  "exportCommands"
};

// FIXME: compilation fails without inlining
// needs to be moved in an implementation file
inline bool
isMachineSpecificServicesUMS(unsigned id) {
  return false;
}


#endif  // _UMSSERVICES_HPP_
