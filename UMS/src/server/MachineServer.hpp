#ifndef MACHINE_SERVER_H
#define MACHINE_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionServer.hh"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

#include "utilServer.hh"
using namespace utilServer;

//extern std::string utilServer::mvishnuid;


class MachineServer {

public:
  MachineServer(UMS_Data::Machine*& machine, SessionServer& session);
  int 
  add();
  int 
  update();
  int 
  deleteMachine();
  ~MachineServer();
  UMS_Data::Machine*
  getData();
  //static UMS_Data::ListMachines list(SessionServer session, UMS_Data::LisMachineOptions options);
  std::string 
  getAttribut(std::string condition, std::string attrname = "nummachineid");
  
private:
  UMS_Data::Machine *mmachine;
  SessionServer msessionServer;
  DbFactory factory;
  Database *mdatabaseVishnu;
 
private:
  int generateMachineId();
};
#endif
