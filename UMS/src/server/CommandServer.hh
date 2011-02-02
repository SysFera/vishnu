#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionServer.hh"
#include "UMS_Data/Command.hpp"
#include "UMS_Data/ListCommands.hpp"
#include "UMS_Data/ListCmdOptions.hpp"

class CommandServer
{
private:
	UMS_Data::Command command;
	SessionServer sessionServer;
 
public:
	void CommandServer(UMS_Data::Command cmd, SessionServer session);
	int record();
	int delete();
	void ~CommandServer();
	UMS_Data::Command getData();
	//static UMS_Data::ListCommands  list(SessionServer session, UMS_Data::ListCmdOptions  options);
};
#endif
