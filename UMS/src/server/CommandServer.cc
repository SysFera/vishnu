#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "CommandServer.h"


void CommandServer::CommandServer(UMS_Data::Command cmd, SessionServer session)
{
}
 
int CommandServer::record()
{
	return 0;
}
 
int CommandServer::delete()
{
	return 0;
}
 
void CommandServer::~CommandServer()
{
}
 
UMS_Data::Command CommandServer::getData()
{
	return 0;
}
 
UMS_Data::ListCommands  CommandServer::list(SessionServer session, UMS_Data::ListCmdOptions  options)
{
	return 0;
}
