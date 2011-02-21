/**
* \file MachineServer.hpp
* \brief This file presents the Class which manipulates VISHNU machine data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef MACHINE_SERVER_H
#define MACHINE_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <exception>
#include "SystemException.hpp"
#include "UMSVishnuException.hpp"
#include "SessionServer.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include"utilServer.hpp"

using namespace utilServer;

/**
* \class MachineServer
* \brief Implementation of the MachineServer class
*/
class MachineServer {

public:
  /**
  * \brief Constructor
  * \fn MachineServer(UMS_Data::Machine*& machine)
  * \param machine The machine data structure
  */
  MachineServer(UMS_Data::Machine*& machine);
  /**
  * \brief Constructor
  * \fn MachineServer(UMS_Data::Machine*& machine, SessionServer& session)
  * \param machine The machine data structure
  * \param session The object which encapsulates session data
  */
  MachineServer(UMS_Data::Machine*& machine, SessionServer& session);
  /**
  * \brief Function to add a new VISHNU machine
  * \fn int add() 
  * \return raises an exception on error
  */
  int 
  add();
  /**
  * \brief Function to update a VISHNU machine
  * \fn int update() 
  * \return raises an exception on error
  */
  int 
  update();
  /**
  * \brief Function to delete a VISHNU machine
  * \fn int deleteMachine() 
  * \return raises an exception on error
  */
  int 
  deleteMachine();
  /**
  * \fn ~MachineServer()
  * \brief Destructor
  */
  ~MachineServer();
  /**
  * \brief Function to get machine information
  * \fn UMS_Data::Machine* getData()
  * \return  The user data structure
  */
  UMS_Data::Machine*
  getData();
  /**
  * \brief Function to get machine information from the database vishnu
  * \fn getAttribut(std::string condition, std::string attrname);
  * \param condition The condition of the select request
  * \param attrname the name of the attribut to get
  * \return the value of the attribut or empty string if no results
  */
  std::string 
  getAttribut(std::string condition, std::string attrname = "nummachineid");  
  /**
  * \brief Function to get the content of the public ssh key
  * \fn std::string getPublicKey()
  * \return The content of the ssh public key
  */
  std::string
  getPublicKey();
  
private:
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The machine data structure
  */
  UMS_Data::Machine *mmachine;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer msessionServer;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;
  /**
  * \brief The content of the ssh public key
  */
  std::string msshpublickey;
  
  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
  * \brief Function to generate a machine id
  * \fn generateMachineId()
  */
  std::string 
  generateMachineId();
};
#endif
