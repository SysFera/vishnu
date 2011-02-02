#ifndef MACHINE_CLIENT_SERVER_H
#define MACHINE_CLIENT_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//#include "SessionServer.hh"

class MachineClientServer {
  
public:
	MachineClientServer(std::string sshKey, std::string host);
	int recordMachineClient();
	int getId();
	std::string getSSHKey() const;
	std::string getHost() const;
	//~MachineClientServer();
private:
	std::string mmachineSSHKey;
	std::string mhostname;
	Database *mdatabaseVishnu;
	DbFactory factory;
 };
#endif//MACHINE_CLIENT_SERVER_H 
