#ifndef MACHINE_CLIENT_SERVER_H
#define MACHINE_CLIENT_SERVER_H

//#include "SessionServer.hh"

#include "POSTGREDatabase.hh"
#include "DbFactory.hh"
#include "DatabaseResult.hh"


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
	bool exist();
 };
#endif//MACHINE_CLIENT_SERVER_H 
