#ifndef CONFIGURATION_SERVER_H
#define CONFIGURATION_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <ecore.hpp> 
#include <ecorecpp.hpp>
#include "SessionServer.hh"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

class ConfigurationServer {
 
public:
  ConfigurationServer(SessionServer session);
  ConfigurationServer(UMS_Data::Configuration*& configuration, SessionServer session);
  int 
  save();
  int 
  restore();
  ~ConfigurationServer();
  UMS_Data::Configuration* 
  getConfiguration();
private:
  //UMS_Data::UMS_DataFactory_ptr mecoreFactory;
  UMS_Data::Configuration *mconfiguration;
  SessionServer msessionServer;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;
  
  std::string
  userToSql(UMS_Data::User_ptr user);
  std::string
  machineToSql(UMS_Data::Machine_ptr machine);
  std::string
  machineDescToSql(UMS_Data::Machine_ptr machine);
  
  std::string
  localAccountToSql(UMS_Data::LocalAccount_ptr localAccount);
};
#endif
