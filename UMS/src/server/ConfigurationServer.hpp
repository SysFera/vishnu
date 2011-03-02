/**
* \file ConfigurationServer.hpp
* \brief This file presents the Class which manipulates VISHNU configuration data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/
#ifndef CONFIGURATION_SERVER_H
#define CONFIGURATION_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <ecore.hpp>
#include <ecorecpp.hpp>
#include "SessionServer.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

/**
* \class ConfigurationServer
* \brief Implementation of the ConfigurationServer class
*/
class ConfigurationServer {

public:
  /**
  * \brief Constructor
  * \fn ConfigurationServer(SessionServer session)
  * \param session The object which encapsulates session data
  */
  ConfigurationServer(SessionServer session);
  /**
  * \brief Constructor
  * \fn ConfigurationServer(Configuration*& configuration, SessionServer session);
  * \param configuration The Configuration data structure
  * \param session The object which encapsulates session data
  */
  ConfigurationServer(UMS_Data::Configuration*& configuration, SessionServer session);
  /**
  * \brief Function to save a VISHNU configuration
  * \fn int save()
  * \return raises an exception on error
  */
  int
  save();
  /**
  * \brief Function to restore a VISHNU configuration
  * \fn int restore()
  * \return raises an exception on error
  */
  int
  restore();
  /**
  * \fn ~ConfigurationServer()
  * \brief Destructor
  */
  ~ConfigurationServer();
  /**
  * \brief Function to get Configuration data structure
  * \fn UMS_Data::Configuration*  getData()
  * \return  The LocalAccount data structure
  */
  UMS_Data::Configuration*
  getData();

private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The Configuration data structure
  */
  UMS_Data::Configuration *mconfiguration;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer msessionServer;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
  * \brief Function to get the sql code of users from a VISHNU configuration
  * \fn std::string userToSql(UMS_Data::User_ptr user)
  * \param user The user object
  * \return the sql code containing the sql code of users
  */
  std::string
  userToSql(UMS_Data::User_ptr user);
  /**
  * \brief Function to get the sql code of machines from a VISHNU configuration
  * \fn std::string machineToSql(UMS_Data::Machine_ptr machine)
  * \param machine The machine object
  * \return the sql code containing the sql code of machines
  */
  std::string
  machineToSql(UMS_Data::Machine_ptr machine);
  /**
  * \brief Function to get the sql code of machines description from a VISHNU configuration
  * \fn std::string machineDescToSql(UMS_Data::Machine_ptr machine)
  * \param machine The machine object
  * \return the sql code containing the sql code of machines description
  */
  std::string
  machineDescToSql(UMS_Data::Machine_ptr machine);
};
#endif
