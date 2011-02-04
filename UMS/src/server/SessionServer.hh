#ifndef SESSION_SERVER_H
#define SESSION_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserServer.hh"
#include "MachineClientServer.hh"

#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "UMS_Data.hpp"

using namespace UMS_Data;

/*
#include "UMS_Data/Session.hpp"
#include "UMS_Data/ConnectOptions.hpp"
#include "UMS_Data/ListSessions.hpp"
#include "UMS_Data/ListSessionOptions.hpp"
*/
class SessionServer
{
private:
	UMS_Data::Session msession;
	UserServer* muserServer;
	DbFactory factory;
	Database *mdatabaseVishnu;
 
private:
	int generateSessionKey(std::string salt);
	int generateSessionId(std::string userId);
	int checkClientMachine(MachineClientServer MachineClient);
	int recordSessionServer(std::string idmachine, std::string iduser);
	int exist();
public:
	SessionServer(std::string sessionKey);
	SessionServer(UMS_Data::Session session);
	int connectSession(UserServer user, MachineClientServer host, std::string opt);
	int reconnect();
	int close();
	~SessionServer();
	UMS_Data::Session getData();
	//static UMS_Data::ListSessions  list(SessionServer session, UMS_Data::ListSessionOptions  options);
};
#endif
