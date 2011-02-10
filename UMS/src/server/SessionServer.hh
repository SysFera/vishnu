#ifndef SESSION_SERVER_H
#define SESSION_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "UMS_Data.hpp"

class UserServer;
#include "UserServer.hh"
#include "MachineClientServer.hh"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
//#include "utilServer.hh"
#include "OptionValueServer.hh"

using namespace UMS_Data;


class SessionServer {
public:
  SessionServer();
  SessionServer(std::string sessionKey);
  SessionServer(const UMS_Data::Session& session);
  int 
  connectSession(UserServer user, MachineClientServer host, UMS_Data::ConnectOptions* connectOpt);
  int 
  reconnect(UserServer user, MachineClientServer host, std::string sessionId);
  int 
  close();
  ~SessionServer();
  UMS_Data::Session 
  getData();
  std::string 
  getAttribut(std::string condition, std::string attrname="sessionkey");
  //static std::vector<std::vector <std::string> > get(std::string sqlcode);
  //static UMS_Data::ListSessions  list(SessionServer session, UMS_Data::ListSessionOptions  options);

  private:
  UMS_Data::Session msession;
  //UserServer muserServer;
  DbFactory factory;
  Database* mdatabaseVishnu;

  private:
  int 
  generateSessionKey(std::string salt);
  int 
  generateSessionId(std::string userId);
  int 
  checkClientMachine(MachineClientServer MachineClient);
  int 
  recordSessionServer(std::string idmachine, std::string iduser);
  bool 
  exist(bool flagSessionId = false);
  int 
  getState(bool flagSessionId = false);
  int 
  getSessionkey(std::string idmachine, std::string iduser, bool flagAdmin = false);
  int 
  solveConnectionMode(UMS_Data::ConnectOptions* connectOpt, UserServer user);
    
  
};
#endif
