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

#include "UserServer.hh"
#include "MachineClientServer.hh"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "utilServer.hh"


using namespace UMS_Data;


class SessionServer
{
private:
	UMS_Data::Session msession;
	UserServer* muserServer;
	DbFactory factory;
	Database* mdatabaseVishnu;
 
private:
	int generateSessionKey(std::string salt);
	int generateSessionId(std::string userId);
	int checkClientMachine(MachineClientServer MachineClient);
	int recordSessionServer(std::string idmachine, std::string iduser);
	int exist();
	int getState();
	int getSessionkey(std::string idmachine);
public:
	SessionServer(std::string sessionKey);
	SessionServer(UMS_Data::Session session);
	int connectSession(UserServer user, MachineClientServer host, std::string opt);
	int reconnect(UserServer user, MachineClientServer host, std::string sessionId);
	int close();
	~SessionServer();
	UMS_Data::Session getData();
	//static std::vector<std::vector <std::string> > get(std::string sqlcode);
	//static UMS_Data::ListSessions  list(SessionServer session, UMS_Data::ListSessionOptions  options);
};
#endif
