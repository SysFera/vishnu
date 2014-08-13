/**
* \file SessionServer.hpp
* \brief This file presents the Class which manipulates VISHNU sessions on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef SESSION_SERVER_H
#define SESSION_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <ecore.hpp>
#include <ecorecpp.hpp>
#include <iostream>
#include <exception>
#include <boost/scoped_ptr.hpp>
#include "VishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "SystemException.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "UserServer.hpp"
#include "MachineClientServer.hpp"
#include "OptionValueServer.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"


class UserServer;

using namespace vishnu;
using namespace UMS_Data;
using namespace boost::posix_time;
using namespace boost::gregorian;

/**
* \class SessionServer
* \brief UserServer class implementation
*/
class SessionServer {
public:
  SessionServer() {}
  SessionServer(std::string sessionKey) {}
  ~SessionServer() {}

  SessionServer(const UMS_Data::Session& session) {}
  int connectSession(UserServer user, MachineClientServer host, UMS_Data::ConnectOptions* connectOpt) {}
  int reconnect(UserServer user, MachineClientServer host, std::string sessionId) {}
  int close() {}

  UMS_Data::Session getData() const {}

  std::string getAttribut(std::string condition, std::string attrname="sessionkey") {}

  int saveConnection() {}

  DatabaseResult* getSessionToclosebyTimeout() {}
  int check();

  int finish (std::string cmdDescription, CmdType cmdType, CmdStatus cmdStatus, std::string newVishnuObjectID = "") {}

  private:
  UMS_Data::Session msession;
  Database* mdatabaseVishnu;

  int generateSessionKey(std::string salt) {}
  int generateSessionId(std::string userId) {}
  int recordSessionServer(std::string idmachine, std::string iduser) {}
  bool exist(bool flagSessionId = false) {}
  int getState(bool flagSessionId = false) {}
  int getSessionkey(std::string idmachine, std::string iduser, bool flagAdmin = false) {}
  int solveConnectionMode(UMS_Data::ConnectOptions* connectOpt, std::string numuserId) {}
  int disconnetToTimeout(UserServer user) {}
};
#endif
